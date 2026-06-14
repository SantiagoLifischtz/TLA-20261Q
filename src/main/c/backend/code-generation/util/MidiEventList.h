#ifndef MIDI_EVENT_LIST_HEADER
#define MIDI_EVENT_LIST_HEADER

#include <stdint.h>
#include <stdbool.h>

typedef struct MidiEventListCDT * MidiEventListADT;

typedef struct {
	uint32_t ticks;
	uint8_t * data;
	uint32_t length;
} MidiEventData;

typedef struct {
	bool succeeded;
	MidiEventData event;
} MidiEventResult;

MidiEventListADT midiEventListNew(void);

void midiEventListFree(MidiEventListADT midiEventList);

bool midiEventListAppend(MidiEventListADT midiEventList, uint32_t ticks, uint8_t * data, uint32_t length);

bool midiEventListPrepend(MidiEventListADT midiEventList, uint32_t ticks, uint8_t * data, uint32_t length);

uint32_t midiEventListGetCount(MidiEventListADT midiEventList);
uint32_t midiEventListGetLastTick(MidiEventListADT midiEventList);

bool midiEventListHasNext(MidiEventListADT midiEventList);

MidiEventResult midiEventListNext(MidiEventListADT midiEventList);

void midiEventListResetIterator(MidiEventListADT midiEventList);

#endif
