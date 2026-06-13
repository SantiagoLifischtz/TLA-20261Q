#ifndef PATTERN_PROCESSOR_HEADER
#define PATTERN_PROCESSOR_HEADER

#include <stdint.h>


// TODO estructuras de datos de tracks, et al

typedef struct MidiEvent {
	uint32_t ticks;
	uint8_t * data;
	uint32_t length;
	struct MidiEvent * next;
} MidiEvent;


// TODO
// processNote(), processPattern(), et al 

#endif
