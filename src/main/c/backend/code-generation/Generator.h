#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/language/String.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "DefinitionTables.h"
#include "PlayBlock.h"
#include "PatternProcessor.h"
#include "midi/MidiEvents.h"
#include "midi/MidiInstruments.h"
#include "midi/MidiConstants.h"
#include "util/MidiEventList.h"
#include "util/Buffer.h"
#include "util/ExpressionEvaluator.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Initialize module's internal state. */
ModuleDestructor initializeGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 * @return true on success, false on semantic or generation failure.
 */
bool executeGenerator(CompilerState * compilerState);

#endif
