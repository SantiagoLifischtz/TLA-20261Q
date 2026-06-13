#ifndef MIDI_INSTRUMENTS_HEADER
#define MIDI_INSTRUMENTS_HEADER

#include <stdint.h>

/**
 * @ref https://midimusic.github.io/tech/midispec.html#BMA1_4
 */
uint8_t getMidiInstrumentNumber(const char * instrumentName);

#endif
