#ifndef PATTERN_PROCESSOR_HEADER
#define PATTERN_PROCESSOR_HEADER

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include "DefinitionTables.h"
#include "midi/MidiEvents.h"
#include "midi/MidiInstruments.h"
#include "midi/MidiNotes.h"
#include "util/MidiEventList.h"
#include "util/ExpressionEvaluator.h"

void setPatternProcessorLogger(Logger * logger);
void setPatternProcessorConductorTrack(MidiEventListADT track);

uint32_t processNote(MidiEventListADT events, Note * note, uint32_t playhead, Scale * scale, unsigned char channel);
uint32_t processRest(Rest * rest);
uint32_t processNoteChord(MidiEventListADT events, NoteChord * chord, uint32_t playhead, Scale * scale, unsigned char channel);
uint32_t processStrum(MidiEventListADT events, Strum * strum, uint32_t playhead, Scale * scale, unsigned char channel);
uint32_t processArpeggio(MidiEventListADT events, Arpeggio * arp, uint32_t playhead, Scale * scale, unsigned char channel);
uint32_t processBlock(MidiEventListADT events, Block * block, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);
uint32_t processPatternId(MidiEventListADT events, Identifier * id, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);
uint32_t processRepeat(MidiEventListADT events, Repeat * repeat, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);
uint32_t processStep(MidiEventListADT events, Step * step, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);
uint32_t processPatternChord(MidiEventListADT events, PatternChord * chord, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);

uint32_t dispatchPatternSentence(MidiEventListADT events, PatternSentence * ps, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);

#endif
