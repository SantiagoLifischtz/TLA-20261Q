#include "ScaleUtils.h"

// Filas = modos 0=major…6=locrian
// Columnas = semitonos grados 1–7
static const char MODE_INTERVALS[7][7] = {
	{0, 2, 4, 5, 7, 9, 11},
	{0, 2, 3, 5, 7, 9, 10},
	{0, 1, 3, 5, 7, 8, 10},
	{0, 2, 4, 6, 7, 9, 11},
	{0, 2, 4, 5, 7, 9, 10},
	{0, 2, 3, 5, 7, 8, 10},
	{0, 1, 3, 5, 6, 8, 10},
};

static void _setScale(Scale * scale, const char root, const char mode) {
	scale->root = root;
	scale->mode = mode;
	for (int degree = 1; degree <= 7; ++degree) {
		char interval = MODE_INTERVALS[(unsigned char) mode][degree - 1];
		scale->degreeToSemitone[degree] = root + interval;
	}
}

void applyKeyToScale(Key * key, Scale * scale) {
	if (key == NULL || key->note == NULL || key->mode == NULL || scale == NULL) {
		return;
	}
	_setScale(scale, key->note->value, key->mode->value);
}

//Si no hay escala, que tenga un default de C major
void setDefaultScale(Scale * scale) {
	_setScale(scale, 0, 0);
}
