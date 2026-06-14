#include "ExpressionEvaluator.h"

static FloatEvaluation _failedEvaluation(void) {
	FloatEvaluation evaluation = { .succeeded = false, .value = 0.0f };
	return evaluation;
}

FloatEvaluation evaluateNumber(Number * number) {
	if (number == NULL) {
		return _failedEvaluation();
	}
	FloatEvaluation evaluation = { .succeeded = true, .value = 0.0f };
	switch (number->type) {
		case INTEGER:
			evaluation.value = (float) number->intValue;
			break;
		case FLOAT:
			evaluation.value = number->floatValue;
			break;
		default:
			return _failedEvaluation();
	}
	return evaluation;
}

FloatEvaluation evaluateFactor(Factor * factor) {
	if (factor == NULL) {
		return _failedEvaluation();
	}
	switch (factor->type) {
		case NUMBER:
			return evaluateNumber(factor->number);
		case EXPRESSION:
			return evaluateExpression(factor->expression);
		default:
			return _failedEvaluation();
	}
}

FloatEvaluation evaluateExpression(Expression * expression) {
	if (expression == NULL) {
		return _failedEvaluation();
	}
	switch (expression->type) {
		case ADDITION:
		case SUBTRACTION:
		case MULTIPLICATION:
		case DIVISION: {
			FloatEvaluation left = evaluateExpression(expression->leftExpression);
			FloatEvaluation right = evaluateExpression(expression->rightExpression);
			if (!left.succeeded || !right.succeeded) {
				return _failedEvaluation();
			}
			FloatEvaluation evaluation = { .succeeded = true, .value = 0.0f };
			switch (expression->type) {
				case ADDITION:
					evaluation.value = left.value + right.value;
					break;
				case SUBTRACTION:
					evaluation.value = left.value - right.value;
					break;
				case MULTIPLICATION:
					evaluation.value = left.value * right.value;
					break;
				case DIVISION:
					if (right.value == 0.0f) {
						return _failedEvaluation();
					}
					evaluation.value = left.value / right.value;
					break;
				default:
					break;
			}
			return evaluation;
		}
		case DOT: {
			FloatEvaluation inner = evaluateExpression(expression->singleExpression);
			if (!inner.succeeded) {
				return _failedEvaluation();
			}
			inner.value *= 1.5f;
			return inner;
		}
		case FACTOR:
			return evaluateFactor(expression->factor);
		default:
			return _failedEvaluation();
	}
}

uint32_t evaluateDurationToTicks(Expression * expression) {
	FloatEvaluation eval = evaluateExpression(expression);

	if (!eval.succeeded || eval.value <= 0) {
		return 0; // 0 duration is error in MIDI
	}

	return (uint32_t) (eval.value*4 * MIDI_TICKS_PER_QUARTER);
}