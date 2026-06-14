#include "MidiEventList.h"

#include <stdlib.h>
#include <string.h>

typedef struct MidiEvent {
	uint32_t ticks;
	uint8_t * data;
	uint32_t length;
	struct MidiEvent * next;
} MidiEvent;

typedef struct MidiEventListCDT {
	uint32_t count;
	uint32_t lastTick;
	MidiEvent * head;
	MidiEvent * tail;
	MidiEvent * current;
} MidiEventListCDT;

MidiEventListADT midiEventListNew(void) {
	return calloc(1, sizeof(MidiEventListCDT));
}

void midiEventListFree(MidiEventListADT midiEventList) {
	if (midiEventList == NULL) {
		return;
	}

	MidiEvent * current = midiEventList->head;

	while (current != NULL) {
		MidiEvent * next = current->next;
		free(current->data);
		free(current);
		current = next;
	}

	free(midiEventList);
}

bool midiEventListAppend(MidiEventListADT midiEventList, uint32_t ticks, uint8_t * data, uint32_t length) {
	MidiEvent * midiEvent = malloc(sizeof(MidiEvent));

	if (midiEvent == NULL) {
		return false;
	}

	midiEvent->data = NULL;
	midiEvent->length = length;
	midiEvent->ticks = ticks;
	midiEvent->next = NULL;

	if (length > 0) {
		midiEvent->data = malloc(length);
		memcpy(midiEvent->data, data, length);
	}

	if (midiEventList->tail == NULL) {
		midiEventList->head = midiEventList->tail = midiEvent;
	}
	else {
		midiEventList->tail->next = midiEvent;
		midiEventList->tail = midiEvent;
	}

	midiEventList->count++;
	
	if (ticks > midiEventList->lastTick) {
		midiEventList->lastTick = ticks;
	}
	
	return true;
}

uint32_t midiEventListGetCount(MidiEventListADT midiEventList) {
	return midiEventList->count;
}

uint32_t midiEventListGetLastTick(MidiEventListADT midiEventList) {
	return midiEventList->lastTick;
}

bool midiEventListHasNext(MidiEventListADT midiEventList) {
	return midiEventList->current != NULL;
}

void midiEventListResetIterator(MidiEventListADT midiEventList) {
	midiEventList->current = midiEventList->head;
}

MidiEventResult midiEventListNext(MidiEventListADT midiEventList) {
	MidiEvent * midiEvent = midiEventList->current;

	MidiEventResult result;
	result.succeeded = midiEvent != NULL;
	if (midiEvent != NULL) {
		result.event.ticks = midiEvent->ticks;
		result.event.data = midiEvent->data;
		result.event.length = midiEvent->length;
		midiEventList->current = midiEvent->next;
	}

	return result;
}
