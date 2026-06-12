#include "DefinitionTables.h"

#include "../../support/logging/Logger.h"
#include <stdlib.h>
#include <string.h>

static Logger * _logger = NULL;

// 120 bpm por default 
static const float DEFAULT_TEMPO_BPM = 120.0f;

//el canal 10 lo reservamos para percusión
static const unsigned char PERCUSSION_CHANNEL = 9;

static const unsigned char CHANNEL_COUNT = 16;


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

typedef struct {
	bool succeeded;
	float value;
} FloatEvaluation;


static FloatEvaluation _evaluateExpression(Expression * expression);
static FloatEvaluation _evaluateFactor(Factor * factor);
static FloatEvaluation _evaluateNumber(Number * number);
static void _setScale(Scale * scale, const char root, const char mode);
static void _setDefaultScale(Scale * scale);
static bool _findPattern(const DefinitionContext * context, const char * name);
static bool _findTrack(const DefinitionContext * context, const char * name);
static bool _insertPattern(DefinitionContext * context, PatternDefinition * pattern);
static bool _insertTrack(DefinitionContext * context, Track * track);
static bool _assignInstrumentChannel(DefinitionContext * context, const char * instrumentName);
static bool _isChannelAvailable(const DefinitionContext * context, unsigned char channel);
static DefinitionTablesResult _failedResult(void);
static DefinitionTablesResult _succeededResult(void);

static FloatEvaluation _failedEvaluation(void) {
	FloatEvaluation evaluation = { .succeeded = false, .value = 0.0f };
	return evaluation;
}

static FloatEvaluation _evaluateNumber(Number * number) {
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

static FloatEvaluation _evaluateFactor(Factor * factor) {
	if (factor == NULL) {
		return _failedEvaluation();
	}
	switch (factor->type) {
		case NUMBER:
			return _evaluateNumber(factor->number);
		case EXPRESSION:
			return _evaluateExpression(factor->expression);
		default:
			return _failedEvaluation();
	}
}

static FloatEvaluation _evaluateExpression(Expression * expression) {
	if (expression == NULL) {
		return _failedEvaluation();
	}
	switch (expression->type) {
		case ADDITION:
		case SUBTRACTION:
		case MULTIPLICATION:
		case DIVISION: {
			FloatEvaluation left = _evaluateExpression(expression->leftExpression);
			FloatEvaluation right = _evaluateExpression(expression->rightExpression);
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
						logError(_logger, "Division by zero while evaluating tempo expression.");
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
			FloatEvaluation inner = _evaluateExpression(expression->singleExpression);
			if (!inner.succeeded) {
				return _failedEvaluation();
			}
			inner.value *= 1.5f;
			return inner;
		}
		case FACTOR:
			return _evaluateFactor(expression->factor);
		default:
			return _failedEvaluation();
	}
}


static void _setScale(Scale * scale, const char root, const char mode) {
	scale->root = root;
	scale->mode = mode;
	for (int degree = 1; degree <= 7; ++degree) {
		const char interval = MODE_INTERVALS[(unsigned char) mode][degree - 1];
		scale->degreeToSemitone[degree] = (char) ((root + interval) % 12);
	}
}

//Si no hay escala, que tenga un default de C major
static void _setDefaultScale(Scale * scale) {
	_setScale(scale, 0, 0);
}

static bool _findPattern(const DefinitionContext * context, const char * name) {
	for (PatternEntry * entry = context->patterns; entry != NULL; entry = entry->next) {
		if (strcmp(entry->id->name, name) == 0) {
			return true;
		}
	}
	return false;
}

//Misma regla de ID duplicado para tracks
static bool _findTrack(const DefinitionContext * context, const char * name) {
	for (TrackEntry * entry = context->tracks; entry != NULL; entry = entry->next) {
		if (strcmp(entry->id->name, name) == 0) {
			return true;
		}
	}
	return false;
}

// Linked list de definiciones por ID 
static bool _insertPattern(DefinitionContext * context, PatternDefinition * pattern) {
	PatternEntry * entry = calloc(1, sizeof(PatternEntry));
	if (entry == NULL) {
		logError(_logger, "Out of memory while building pattern table.");
		return false;
	}
	entry->id = pattern->id;
	entry->definition = pattern;
	entry->next = context->patterns;
	context->patterns = entry;
	return true;
}

static bool _insertTrack(DefinitionContext * context, Track * track) {
	TrackEntry * entry = calloc(1, sizeof(TrackEntry));
	if (entry == NULL) {
		logError(_logger, "Out of memory while building track table.");
		return false;
	}
	entry->id = track->id;
	entry->track = track;
	entry->next = context->tracks;
	context->tracks = entry;
	return true;
}

static bool _isChannelAvailable(const DefinitionContext * context, unsigned char channel) {
	for (InstrumentChannelEntry * entry = context->instrumentChannels; entry != NULL; entry = entry->next) {
		if (entry->channel == channel) {
			return false;
		}
	}
	return true;
}

static bool _assignInstrumentChannel(DefinitionContext * context, const char * instrumentName) {
	if (instrumentName == NULL) {
		logError(_logger, "Track is missing an instrument declaration.");
		return false;
	}
	// Si se repite el instrumento, se asigna el mismo canal
	for (InstrumentChannelEntry * entry = context->instrumentChannels; entry != NULL; entry = entry->next) {
		if (strcmp(entry->instrumentName, instrumentName) == 0) {
			return true;
		}
	}
	for (unsigned char channel = 0; channel < CHANNEL_COUNT; ++channel) {
		if (channel == PERCUSSION_CHANNEL) {
			continue;
		}
		if (!_isChannelAvailable(context, channel)) {
			continue;
		}
		InstrumentChannelEntry * entry = calloc(1, sizeof(InstrumentChannelEntry));
		if (entry == NULL) {
			logError(_logger, "Out of memory while building instrument channel table.");
			return false;
		}
		entry->instrumentName = strdup(instrumentName);
		if (entry->instrumentName == NULL) {
			free(entry);
			logError(_logger, "Out of memory while building instrument channel table.");
			return false;
		}
		entry->channel = channel;
		entry->next = context->instrumentChannels;
		context->instrumentChannels = entry;
		return true;
	}
	//Si no hay un canal libre, falla
	logError(_logger, "No free MIDI channel available for instrument \"%s\".", instrumentName);
	return false;
}

//Falla la compilación ante un error semántico
static DefinitionTablesResult _failedResult(void) {
	DefinitionTablesResult result = { .succeeded = false };
	return result;
}

static DefinitionTablesResult _succeededResult(void) {
	DefinitionTablesResult result = { .succeeded = true };
	return result;
}


// Libera entradas de tabla; no destruye nodos del AST
void destroyDefinitionContext(DefinitionContext * context) {
	if (context == NULL) {
		return;
	}
	for (PatternEntry * pattern = context->patterns; pattern != NULL; ) {
		PatternEntry * next = pattern->next;
		free(pattern);
		pattern = next;
	}
	for (TrackEntry * track = context->tracks; track != NULL; ) {
		TrackEntry * next = track->next;
		free(track);
		track = next;
	}
	for (InstrumentChannelEntry * instrument = context->instrumentChannels; instrument != NULL; ) {
		InstrumentChannelEntry * next = instrument->next;
		free(instrument->instrumentName);
		free(instrument);
		instrument = next;
	}
	memset(context, 0, sizeof(DefinitionContext));
}


DefinitionTablesResult buildDefinitionContext(const Program * program, DefinitionContext * context) {
	if (_logger == NULL) {
		_logger = createLogger("DefinitionTables");
	}
	if (program == NULL || context == NULL) {
		logError(_logger, "Cannot build definition tables from a null program or context.");
		return _failedResult();
	}
	memset(context, 0, sizeof(DefinitionContext));

	for (Definitions * definitions = program->definitions; definitions != NULL; definitions = definitions->next) {
		Definition * definition = definitions->def;
		if (definition == NULL) {
			continue;
		}
		switch (definition->type) {
			case CONFIG: {
				ConfigSentence * config = definition->config;
				if (config == NULL) {
					continue;
				}
				if (config->type == TEMPO) {
					if (context->globalTempoSet) {
						logError(_logger, "Global tempo was already defined.");
						return _failedResult();
					}
					FloatEvaluation tempo = _evaluateExpression(config->tempo->expression);
					if (!tempo.succeeded || tempo.value <= 0.0f) {
						logError(_logger, "Invalid global tempo expression.");
						return _failedResult();
					}
					context->globalTempoBpm = tempo.value;
					context->globalTempoSet = true;
				}
				else if (config->type == KEY) {
					//si se trata de pisar la escala antes de analizar tracks, falla
					if (context->globalScaleSet) {
						logError(_logger, "Global key was already defined.");
						return _failedResult();
					}
					Key * key = config->key;
					if (key == NULL || key->note == NULL || key->mode == NULL) {
						logError(_logger, "Invalid global key definition.");
						return _failedResult();
					}
					_setScale(&context->globalScale, key->note->value, key->mode->value);
					context->globalScaleSet = true;
				}
				break;
			}
			case PATTERN_DEF: {
				PatternDefinition * pattern = definition->pattern;
				if (pattern == NULL || pattern->id == NULL || pattern->id->name == NULL) {
					logError(_logger, "Invalid pattern definition.");
					return _failedResult();
				}
				//Falla la compilación si aparece un repetido
				if (_findPattern(context, pattern->id->name)) {
					logError(_logger, "Duplicate pattern identifier \"%s\".", pattern->id->name);
					return _failedResult();
				}
				if (!_insertPattern(context, pattern)) {
					return _failedResult();
				}
				break;
			}
			case TRACK_DEF: {
				Track * track = definition->track;
				if (track == NULL || track->id == NULL || track->id->name == NULL) {
					logError(_logger, "Invalid track definition.");
					return _failedResult();
				}
				if (_findTrack(context, track->id->name)) {
					logError(_logger, "Duplicate track identifier \"%s\".", track->id->name);
					return _failedResult();
				}
				if (!_insertTrack(context, track)) {
					return _failedResult();
				}
				//Cada vez que se inserta una track, registra un instrumento en la tabla de canales
				if (!_assignInstrumentChannel(context, track->instrument != NULL ? track->instrument->name : NULL)) {
					return _failedResult();
				}
				break;
			}
			default:
				logError(_logger, "Unknown definition type: %d", definition->type);
				return _failedResult();
		}
	}

	//Si no hay tempo, que sea 120 bpm por default
	if (!context->globalTempoSet) {
		context->globalTempoBpm = DEFAULT_TEMPO_BPM;
	}
	//Si no hay escala, que tenga un default de C major
	if (!context->globalScaleSet) {
		_setDefaultScale(&context->globalScale);
	}

	logDebugging(
		_logger,
		"Definition tables built: tempo=%.2f BPM, key root=%d mode=%d.",
		context->globalTempoBpm,
		context->globalScale.root,
		context->globalScale.mode
	);
	return _succeededResult();
}
