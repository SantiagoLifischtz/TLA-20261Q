#include "Generator.h"

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

static void _writeMThd(FILE * output, uint16_t numTracks);

//
//




static void _writeMThd(FILE * output, uint16_t numTracks) {
	BufferADT buffer = bufferNew();

	bufferAppendData(buffer, MIDI_CHUNK_TYPE_HEADER, 4);
	bufferAppendU32BE(buffer, MIDI_HEADER_CHUNK_LENGTH);
	bufferAppendU16BE(buffer, MIDI_FORMAT_MULTI_TRACK);
	bufferAppendU16BE(buffer, numTracks);
	bufferAppendU16BE(buffer, MIDI_TICKS_PER_QUARTER);
	fwrite(bufferGetData(buffer), 1, bufferGetLength(buffer), output);

	bufferFree(buffer);
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
	_writeMThd(stdout, (uint16_t) (playResult.exportTrackCount + 1));
	// TODO generar contenido MIDI
	destroyDefinitionContext(&context);
	logDebugging(_logger, "Generation is done.");
	return true;
}
