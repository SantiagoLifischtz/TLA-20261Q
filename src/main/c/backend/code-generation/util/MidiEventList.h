#ifndef MIDI_EVENT_LIST_HEADER
#define MIDI_EVENT_LIST_HEADER

#include <stdint.h>
#include <stdbool.h>

typedef struct MidiEventListCDT * MidiEventListADT;

MidiEventListADT midiEventListNew(void);

void midiEventListFree(MidiEventListADT midiEventList);

bool midiEventListAppend(MidiEventListADT midiEventList, uint32_t ticks, uint8_t * data, uint32_t length);

uint32_t midiEventListGetCount(MidiEventListADT midiEventList);

bool midiEventListHasNext(MidiEventListADT midiEventList);

bool midiEventListNext(MidiEventListADT midiEventList, uint32_t * outTicks, uint8_t ** outData, uint32_t * outLength);

#endif
