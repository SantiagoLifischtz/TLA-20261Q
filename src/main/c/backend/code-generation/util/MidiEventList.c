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
	MidiEvent * midiEvent = malloc(sizeof(midiEvent));

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
	
	return true;
}

uint32_t midiEventListGetCount(MidiEventListADT midiEventList) {
	return midiEventList->count;
}

bool midiEventListHasNext(MidiEventListADT midiEventList) {
	if (midiEventList->current == NULL) {
		midiEventList->current = midiEventList->head;
	}

	return midiEventList->current != NULL;
}

bool midiEventListNext(MidiEventListADT midiEventList, uint32_t * outTicks, uint8_t ** outData, uint32_t * outLength) {
	MidiEvent * midiEvent = midiEventList->current;

	if (midiEvent == NULL) {
		return false;
	}

	if (outTicks) {
		*outTicks = midiEvent->ticks;
	}
	if (outData) {
		*outData = midiEvent->data;
	}
	if (outLength) {
		*outLength = midiEvent->length;
	}

	midiEventList->current = midiEvent->next;

	return true;
}
