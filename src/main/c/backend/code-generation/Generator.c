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


/** PUBLIC FUNCTIONS */

void executeGenerator(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	DefinitionContext context;
	DefinitionTablesResult result = buildDefinitionContext(compilerState->abstractSyntaxtTree, &context);
	if (!result.succeeded) {
		logError(_logger, "DefinitionTables build error.");
		return;
	}
	destroyDefinitionContext(&context);
	logDebugging(_logger, "Generation is done.");
}
