#ifndef EXPRESSION_EVALUATOR_HEADER
#define EXPRESSION_EVALUATOR_HEADER

#include "../../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../midi/MidiConstants.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool succeeded;
	float value;
} FloatEvaluation;

FloatEvaluation evaluateNumber(Number * number);
FloatEvaluation evaluateFactor(Factor * factor);
FloatEvaluation evaluateExpression(Expression * expression);
uint32_t evaluateDurationToTicks(Expression * expression);

#endif
