#ifndef EXPRESSION_EVALUATOR_HEADER
#define EXPRESSION_EVALUATOR_HEADER

#include "../../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct {
	bool succeeded;
	float value;
} FloatEvaluation;

FloatEvaluation evaluateNumber(Number * number);
FloatEvaluation evaluateFactor(Factor * factor);
FloatEvaluation evaluateExpression(Expression * expression);

#endif
