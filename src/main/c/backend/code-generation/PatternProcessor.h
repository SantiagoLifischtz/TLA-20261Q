#ifndef PATTERN_PROCESSOR_HEADER
#define PATTERN_PROCESSOR_HEADER

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include "DefinitionTables.h"
#include "midi/MidiEvents.h"
#include "midi/MidiInstruments.h"
#include "midi/MidiNotes.h"
#include "util/MidiEventList.h"
#include "util/ExpressionEvaluator.h"

void setPatternProcessorLogger(Logger * logger);

uint32_t processNote(Note * note, uint32_t playhead, Scale * scale, unsigned char channel, MidiEventListADT events);

#endif
