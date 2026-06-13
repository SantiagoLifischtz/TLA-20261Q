#include "PatternProcessor.h"

uint32_t processNote(Note * note, uint32_t playhead, Scale * scale, unsigned char channel, MidiEventListADT events) {
	if (note == NULL || note->noteAndOctave == NULL || note->duration == NULL) {
		return 0;
	}

	MidiNoteResult noteRes;
	switch (note->noteAndOctave->type) {
		case ABSOLUTE:
			noteRes = getMidiAbsoluteNote(note->noteAndOctave->note->value, note->noteAndOctave->octave);
			break;
		case FROM_DEGREE:
			noteRes = getMidiDegreeNote(note->noteAndOctave->degree->number, note->noteAndOctave->degree->octave, scale);
			break;
		case FROM_STRING:
			noteRes = getMidiPercussionNumber(note->noteAndOctave->identifier);
			break;
		default:
			return 0;
	}
	if (!noteRes.succeeded) {
		return 0;
	}

	uint32_t durationTicks = evaluateDurationToTicks(note->duration->expression);
	if (durationTicks == 0) {
		return 0;
	}

	unsigned char midiChannel = channel;
	if (note->noteAndOctave->type == FROM_STRING) {
		midiChannel = MIDI_PERCUSSION_CHANNEL;
	}

	uint8_t noteOnData[3] = { MIDI_NOTE_ON | midiChannel, noteRes.value, MIDI_DEFAULT_VELOCITY };
	if (!midiEventListAppend(events, playhead, noteOnData, 3)) {
		return 0;
	}

	uint8_t noteOffData[3] = { MIDI_NOTE_OFF | midiChannel, noteRes.value, 0 };
	if (!midiEventListAppend(events, playhead + durationTicks, noteOffData, 3)) {
		return 0;
	}

	return durationTicks;
}

