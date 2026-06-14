#include "MidiEvents.h"

void buildTempoEvent(uint8_t out[6], uint32_t quarterUsecs) {
	out[0] = MIDI_META_EVENT;
	out[1] = MIDI_META_SET_TEMPO;
	out[2] = 0x03;
	out[3] = (quarterUsecs >> 16) & 0xFF;
	out[4] = (quarterUsecs >> 8) & 0xFF;
	out[5] = quarterUsecs & 0xFF;
}
