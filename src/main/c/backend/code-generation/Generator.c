#include "Generator.h"
#include "PatternProcessor.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/** PRIVATE FUNCTIONS */

static bool _writeMThd(FILE * output, uint16_t numTracks);
static bool _generateAllTracks(FILE * output, DefinitionContext * context);
static bool _writeMTrk(MidiEventListADT events, FILE * output);
static bool _processExportTrack(MidiEventListADT events, TrackEntry * t, DefinitionContext * context);
static uint16_t _countExportTracks(DefinitionContext * context);
static bool _initConductorTrack(MidiEventListADT * conductorTrack);
static bool _prependGlobalTempo(MidiEventListADT conductorTrack, DefinitionContext * context);
static bool _buildExportTrackEvents(
	DefinitionContext * context,
	MidiEventListADT * trackEvents,
	uint16_t exportCount,
	uint32_t * maxEndTick
);
static bool _writeExportTracks(
	FILE * output,
	MidiEventListADT conductorTrack,
	MidiEventListADT * trackEvents,
	uint16_t exportCount,
	uint32_t maxEndTick
);

typedef struct {
	MidiEventListADT * trackEvents;
	uint16_t exportCount;
	MidiEventListADT conductorTrack;
} TrackBuildResources;

static void _releaseTrackBuildResources(TrackBuildResources * resources);

static bool _writeMThd(FILE * output, uint16_t numTracks) {
	BufferADT buffer = bufferNew();
	if (buffer == NULL) {
		logError(_logger, "MThd mem alloc fail.");
		return false;
	}

	bufferAppendData(buffer, MIDI_CHUNK_TYPE_HEADER, 4);
	bufferAppendU32BE(buffer, MIDI_HEADER_CHUNK_LENGTH);
	bufferAppendU16BE(buffer, MIDI_FORMAT_MULTI_TRACK);
	bufferAppendU16BE(buffer, numTracks);
	bufferAppendU16BE(buffer, MIDI_TICKS_PER_QUARTER);
	fwrite(bufferGetData(buffer), 1, bufferGetLength(buffer), output);

	bufferFree(buffer);
	return true;
}

static bool _writeMTrk(MidiEventListADT events, FILE * output) {
	BufferADT trackData = bufferNew();
	if (trackData == NULL) {
		logError(_logger, "MTrk mem alloc fail.");
		return false;
	}

	uint32_t previousTicks = 0;
	midiEventListResetIterator(events);

	while (midiEventListHasNext(events)) {
		MidiEventResult result = midiEventListNext(events);

		bufferAppendVariableLength(trackData, result.event.ticks - previousTicks);

		if (result.event.data != NULL && result.event.length > 0) {
			bufferAppendData(trackData, result.event.data, result.event.length);
		}

		previousTicks = result.event.ticks;
	}

	BufferADT chunk = bufferNew();
	if (chunk == NULL) {
		logError(_logger, "MTrk chunk buffer alloc fail.");
		bufferFree(trackData);
		return false;
	}

	bufferAppendData(chunk, MIDI_CHUNK_TYPE_TRACK, 4);
	bufferAppendU32BE(chunk, bufferGetLength(trackData));
	bufferAppendData(chunk, bufferGetData(trackData), bufferGetLength(trackData));
	bufferFree(trackData);

	fwrite(bufferGetData(chunk), 1, bufferGetLength(chunk), output);
	bufferFree(chunk);

	return true;
}

static bool _generateAllTracks(FILE * output, DefinitionContext * context) {
	TrackBuildResources resources = {
		.trackEvents = NULL,
		.exportCount = _countExportTracks(context),
		.conductorTrack = NULL
	};
	uint32_t maxEndTick = 0;
	bool succeeded = false;

	if (resources.exportCount > 0) {
		resources.trackEvents = calloc(resources.exportCount, sizeof(MidiEventListADT));
		if (resources.trackEvents == NULL) {
			logError(_logger, "Track events array mem alloc fail.");
		}
	}

	if ((resources.exportCount == 0 || resources.trackEvents != NULL)
	    && _initConductorTrack(&resources.conductorTrack)
	    && _buildExportTrackEvents(context, resources.trackEvents, resources.exportCount, &maxEndTick)
	    && _prependGlobalTempo(resources.conductorTrack, context)
	    && _writeExportTracks(output, resources.conductorTrack, resources.trackEvents, resources.exportCount, maxEndTick)) {
		succeeded = true;
	}

	_releaseTrackBuildResources(&resources);
	return succeeded;
}

static uint16_t _countExportTracks(DefinitionContext * context) {
	uint16_t exportCount = 0;

	for (TrackEntry * track = context->tracks; track != NULL; track = track->next) {
		if (track->selectedForExport) {
			exportCount++;
		}
	}

	return exportCount;
}

static bool _initConductorTrack(MidiEventListADT * conductorTrack) {
	*conductorTrack = midiEventListNew();
	if (*conductorTrack == NULL) {
		logError(_logger, "Conductor track mem alloc fail.");
		return false;
	}

	setPatternProcessorConductorTrack(*conductorTrack);

	return true;
}

static bool _prependGlobalTempo(MidiEventListADT conductorTrack, DefinitionContext * context) {
	uint8_t tempoData[6];
	buildTempoEvent(tempoData, MIDI_TEMPO_FROM_BPM(context->globalTempoBpm));

	if (!midiEventListPrepend(conductorTrack, 0, tempoData, 6)) {
		logError(_logger, "Global tempo event prepend fail.");
		return false;
	}

	return true;
}

static bool _buildExportTrackEvents(
	DefinitionContext * context,
	MidiEventListADT * trackEvents,
	uint16_t exportCount,
	uint32_t * maxEndTick
) {
	uint16_t trackIndex = 0;
	*maxEndTick = 0;

	for (TrackEntry * t = context->tracks; t != NULL; t = t->next) {
		if (!t->selectedForExport) {
			continue;
		}

		if (t->track == NULL) {
			trackIndex++;
			continue;
		}

		trackEvents[trackIndex] = midiEventListNew();
		if (trackEvents[trackIndex] == NULL) {
			logError(_logger, "Midi event list mem alloc fail.");
			return false;
		}

		if (!_processExportTrack(trackEvents[trackIndex], t, context)) {
			return false;
		}

		uint32_t endTick = midiEventListGetLastTick(trackEvents[trackIndex]);
		if (endTick > *maxEndTick) {
			*maxEndTick = endTick;
		}

		trackIndex++;
	}

	return true;
}

static bool _writeExportTracks(
	FILE * output,
	MidiEventListADT conductorTrack,
	MidiEventListADT * trackEvents,
	uint16_t exportCount,
	uint32_t maxEndTick
) {
	uint8_t eot[3] = { MIDI_META_EVENT, MIDI_META_END_OF_TRACK, 0x00 };

	if (!midiEventListAppend(conductorTrack, maxEndTick, eot, 3)) {
		logError(_logger, "End of Track fail.");
		return false;
	}

	if (!_writeMTrk(conductorTrack, output)) {
		logError(_logger, "Conductor track write fail.");
		return false;
	}

	for (uint16_t i = 0; i < exportCount; i++) {
		if (trackEvents[i] != NULL && !_writeMTrk(trackEvents[i], output)) {
			logError(_logger, "Instrument track %u write fail.", i);
			return false;
		}
	}

	return true;
}

static bool _processExportTrack(MidiEventListADT events, TrackEntry * t, DefinitionContext * context) {
	// MIDI channel resolver, write Program Change at tick 0 (except Drums)
	const char * instrumentName = (t->track->instrument != NULL) ? t->track->instrument->name : NULL;
	unsigned char channel = instrumentName != NULL ? getInstrumentChannel(context, instrumentName) : 0;

	if (instrumentName != NULL && strcmp(instrumentName, DSL_DRUMS_INSTRUMENT_NAME) != 0) {
		MidiInstrumentResult instrumentResult = getMidiInstrumentNumber(instrumentName);

		if (!instrumentResult.succeeded) {
			logError(_logger, "Instrument unknown: %s", instrumentName);

			return false;
		}

		uint8_t patch = instrumentResult.value;
		uint8_t programChange[2] = { MIDI_PROGRAM_CHANGE | channel, patch };

		if (!midiEventListAppend(events, 0, programChange, 2)) {
			logError(_logger, "Program Change event append fail.");

			return false;
		}
	}

	// Sentences loop, with playhead management
	Scale localScale = context->globalScale;
	uint32_t playhead = 0;

	SentencesResult sentenceResult = processSentences(
		events,
		t->track->sentences,
		playhead,
		&localScale,
		channel,
		context,
		true
	);

	if (!sentenceResult.succeeded) {
		return false;
	}

	playhead += sentenceResult.ticksAdvanced;

	// End of Track append
	uint8_t eot[3] = { MIDI_META_EVENT, MIDI_META_END_OF_TRACK, 0x00 };
	if (!midiEventListAppend(events, playhead, eot, 3)) {
		logError(_logger, "End of Track event append fail.");

		return false;
	}

	return true;
}

static void _releaseTrackBuildResources(TrackBuildResources * resources) {
	if (resources == NULL) {
		return;
	}

	if (resources->conductorTrack != NULL) {
		midiEventListFree(resources->conductorTrack);
		resources->conductorTrack = NULL;
	}

	if (resources->trackEvents != NULL) {
		for (uint16_t i = 0; i < resources->exportCount; i++) {
			if (resources->trackEvents[i] != NULL) {
				midiEventListFree(resources->trackEvents[i]);
			}
		}
		free(resources->trackEvents);
		resources->trackEvents = NULL;
	}
}

bool executeGenerator(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");

	setPatternProcessorLogger(_logger);

	DefinitionContext context;
	DefinitionTablesResult definitionResult = buildDefinitionContext(compilerState->abstractSyntaxtTree, &context);
	if (!definitionResult.succeeded) {
		logError(_logger, "DefinitionTables build error.");
		return false;
	}

	PlayBlockResult playResult = resolvePlayBlock(compilerState->abstractSyntaxtTree, &context);
	if (!playResult.succeeded) {
		logError(_logger, "Play block resolution error.");
		destroyDefinitionContext(&context);
		return false;
	}

	if (!_writeMThd(stdout, (uint16_t) (playResult.exportTrackCount + 1))) {
		logError(_logger, "Failed to write MIDI header.");
		destroyDefinitionContext(&context);
		return false;
	}
	
	if (!_generateAllTracks(stdout, &context)) {
		logError(_logger, "MTrk generation failed.");
		destroyDefinitionContext(&context);
		return false;
	}

	destroyDefinitionContext(&context);
	logDebugging(_logger, "Generation is done.");

	return true;
}
