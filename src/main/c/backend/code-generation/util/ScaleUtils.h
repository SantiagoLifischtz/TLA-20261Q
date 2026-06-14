#ifndef SCALE_UTILS_HEADER
#define SCALE_UTILS_HEADER

#include "../../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

// degreeToSemitone[1-7] semitona por grado
typedef struct Scale {
	char root;
	char mode;
	char degreeToSemitone[8];
} Scale;

void applyKeyToScale(Key * key, Scale * scale);
void setDefaultScale(Scale * scale);

#endif
