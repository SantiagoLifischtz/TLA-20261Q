#ifndef MIDI_NOTES_HEADER
#define MIDI_NOTES_HEADER

#include <stdint.h>
#include <stdbool.h>
#include "../DefinitionTables.h"

typedef struct {
	bool succeeded;
	uint8_t value;
} MidiNoteResult;

MidiNoteResult getMidiAbsoluteNote(char noteValue, char octave);

MidiNoteResult getMidiDegreeNote(int degree, int octave, Scale * scale);

#endif
