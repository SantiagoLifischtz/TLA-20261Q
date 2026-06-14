#ifndef MIDI_INSTRUMENTS_HEADER
#define MIDI_INSTRUMENTS_HEADER

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool succeeded;
	uint8_t value;
} MidiInstrumentResult;

typedef struct {
	bool succeeded;
	uint8_t value;
} MidiPercussionResult;

/**
 * @ref https://midimusic.github.io/tech/midispec.html#BMA1_4
 */
MidiInstrumentResult getMidiInstrumentNumber(const char * instrumentName);

/**
 * @ref https://midimusic.github.io/tech/midispec.html#BMA1_5
 */
MidiPercussionResult getMidiPercussionNumber(const char * name);

#endif
