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

	// 
	// TODO flujo general a partir del Docs (lo que entendí)
	//
	
	// General:
	// implementación 1:1:1 track(DLS):channel:track(MIDI) 
	// linked list para ir guardando eventos

	// Init:
	// TODO crear estructura de datos para tracks con: playhead, instrumento/trackNumber/canal, escala local, et al
	// TODO crear lista global de eventos de tempo ("track extra") y set tempo global default (MidiConstants.h) en tick 0
	// TODO crear mapa instrument -> canal (usar MidiInstruments.c)
	// TODO crear estructura de percusión (track 9, esto definido en MidiConstants.h)

	// Loop principal: para cada track (export=true)
	// inicializar estructura de datos para tracks (instrumento, lista vacía eventos, playhead en 0, escala local, et al)

		// por cada sentence del track (switch-case):
			// KEY: pisar escala local, no avanza playhead
			// TEMPO: agregar set tempo a la track global en playhead actual
			// PATTERN:
				// Note:
					// note ID: validar rango 0-127, falla si se va
					// degree: traducir usando escala local, falla si ?
					// string: traducir con tabla de percusion, canal 9 (MidiConstants.h), falla si no existe
					// note on en playhead, note off en playhead + duracion
					// playhead += wait (o duration si wait es null)
				// Rest:
					// playhead += duration, falla si no es positivo
				// Strum: 
					// validar intervalo > 0, #notas*intervalo < total, falla si no
					// notas escalonadas por intervalo, off en cada end time
					// playhead += wait (o end time si wait es null)
				// Arpeggio:
					// validar duracion > 0
					// notas escalonadas, cada una con duracion total / cantidad
					// playhead += wait (o end time si wait es null)
				// Block:
					// procesar recursivamente sentences
					// playhead += wait (o duracion retornada si wait es null)
				// ID (Pattern):
					// fallar si el patron no existe
					// procesar como block
				// Repeat:
					// repetir la sentence N veces
				// Pattern Chord:
					// procesar cada patron, tomar la duracion mas larga
					// playhead += wait (o duracion mas larga si wait es null)
				// Step:
					// sentences ("no wait"?) con intervalo fijo
					// playhead += wait (o end time si wait es null)
				// Note Chord:
					// note on en playhead, note off en playhead + duracion, para cada nota y todo simultaneo
					// playhead += wait (o duration si wait es null)

	// escribir tracks:
		// MTrk, uso de Buffer.c, recorrer eventos, ir leyendo deltas y escribiendo, et al...
		// TODO

	// escribir track de tempo global
		// TODO (misma logica que MTrk)

	// free de todo
		// TODO

	return true;
}

/** PUBLIC FUNCTIONS */

bool executeGenerator(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
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
	// TODO decisión de diseño: fwrite a archivo en particular o a STDOUT y que el usuario haga pipe?
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

	// TODO frees...

	destroyDefinitionContext(&context);
	logDebugging(_logger, "Generation is done.");
	return true;
}
