#ifndef MIDI_INSTRUMENTS_HEADER
#define MIDI_INSTRUMENTS_HEADER

#include <stdint.h>
#include "MidiNotes.h"

/**
 * @ref https://midimusic.github.io/tech/midispec.html#BMA1_4
 */
uint8_t getMidiInstrumentNumber(const char * instrumentName);

/**
 * @ref https://midimusic.github.io/tech/midispec.html#BMA1_5
 */
MidiNoteResult getMidiPercussionNumber(const char * name);

#endif
