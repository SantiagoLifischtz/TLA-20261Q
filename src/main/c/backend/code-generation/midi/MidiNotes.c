#include "MidiNotes.h"

MidiNoteResult getMidiAbsoluteNote(char noteValue, char octave) {
	MidiNoteResult result;
	result.succeeded = false;
	result.value = 0;

	uint8_t note = (uint8_t) (noteValue + 12 * octave); // TODO check
	if (note <= 127) {
		result.succeeded = true;
		result.value = note;
	}

	return result;
}

MidiNoteResult getMidiDegreeNote(int degree, int octave, Scale * scale) {
	MidiNoteResult result;
	result.succeeded = false;
	result.value = 0;

	if (scale != NULL && degree >= 1 && degree <= 7) {
		int accumulated = scale->degreeToSemitone[degree];
		int resultOctave = octave + accumulated / 12;
		int resultSemitone = accumulated % 12;
		uint8_t note = (uint8_t) (resultSemitone + 12 * resultOctave);
		if (note <= 127) {
			result.succeeded = true;
			result.value = note;
		}
	}

	return result;
}
