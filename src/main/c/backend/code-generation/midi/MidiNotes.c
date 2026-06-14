#include "MidiNotes.h"

static bool _isValidMidiNote(uint8_t note) {
	return note <= 127;
}

MidiNoteResult getMidiAbsoluteNote(char noteValue, char octave) {
	MidiNoteResult result;
	result.succeeded = false;
	result.value = 0;

	if (noteValue < 0 || noteValue > 11 || octave < 0) {
		return result;
	}

	int pitch = noteValue + 12 * (int) octave;
	if (pitch < 0 || pitch > 127) {
		return result;
	}

	uint8_t note = (uint8_t) pitch;
	if (!_isValidMidiNote(note)) {
		return result;
	}

	result.succeeded = true;
	result.value = note;
	return result;
}

MidiNoteResult getMidiDegreeNote(int degree, int octave, Scale * scale) {
	MidiNoteResult result;
	result.succeeded = false;
	result.value = 0;

	if (scale == NULL || degree < 1 || degree > 7 || octave < 0) {
		return result;
	}

	int accumulated = scale->degreeToSemitone[degree];
	int resultOctave = octave + accumulated / 12;
	int resultSemitone = accumulated % 12;
	int pitch = resultSemitone + 12 * resultOctave;
	if (pitch < 0 || pitch > 127) {
		return result;
	}

	uint8_t note = (uint8_t) pitch;
	if (!_isValidMidiNote(note)) {
		return result;
	}

	result.succeeded = true;
	result.value = note;
	return result;
}
