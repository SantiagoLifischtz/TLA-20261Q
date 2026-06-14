#ifndef MIDI_EVENTS_HEADER
#define MIDI_EVENTS_HEADER

#include <stdint.h>

#define MIDI_NOTE_OFF          0x80
#define MIDI_NOTE_ON           0x90
#define MIDI_PROGRAM_CHANGE    0xC0

#define MIDI_META_EVENT        0xFF
#define MIDI_META_SET_TEMPO    0x51
#define MIDI_META_END_OF_TRACK 0x2F

void buildTempoEvent(uint8_t out[6], uint32_t quarterUsecs);

#endif
