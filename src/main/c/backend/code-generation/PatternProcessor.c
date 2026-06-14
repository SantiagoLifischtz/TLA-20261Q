#include "PatternProcessor.h"
#include "../../support/logging/Logger.h"

static Logger * _logger = NULL;

void setPatternProcessorLogger(Logger * logger) {
	_logger = logger;
}


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
		case FROM_STRING: {
			MidiPercussionResult midiPercussionResult = getMidiPercussionNumber(note->noteAndOctave->identifier);
			noteRes.succeeded = midiPercussionResult.succeeded;
			noteRes.value = midiPercussionResult.value;
			break;
		}
		default:
			return 0;
	}
	if (!noteRes.succeeded) {
		if (note->noteAndOctave->type == FROM_STRING) {
			logError(_logger, "Unknown percussion: %s", note->noteAndOctave->identifier);
		} else if (note->noteAndOctave->type == ABSOLUTE) {
			logError(_logger, "Invalid note: %c%c", note->noteAndOctave->note->value, note->noteAndOctave->octave);
		} else if (note->noteAndOctave->type == FROM_DEGREE) {
			logError(_logger, "Invalid degree: %d in octave %d",
			         note->noteAndOctave->degree->number, note->noteAndOctave->degree->octave);
		}
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

