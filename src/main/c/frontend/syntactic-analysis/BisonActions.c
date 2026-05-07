#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

/* TEMP
_logSyntacticAnalyzerAction(__FUNCTION__);
	= calloc(1, sizeof());
*/

Number * IntegerSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Number * number = calloc(1, sizeof(Number));
	number->intValue = value;
	number->type = INTEGER;
	return number;
}

Number * FloatSemanticAction(const float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Number * number = calloc(1, sizeof(Number));
	number->floatValue = value;
	number->type = FLOAT;
	return number;
}

Expression * BinaryOperationSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * UnaryOperationSemanticAction(Expression * operand, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->singleExpression = operand;
	expression->type = type;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Number * number) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->number = number;
	factor->type = NUMBER;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

Program * ProgramSemanticAction(Definitions * definitions, Play * playBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->definitions = definitions;
	program->playBlock = playBlock;
	return program;
}

Definitions * DefinitionListSemanticAction(Definition * newDefinition, Definitions * otherDefinitions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definitions * definitions = calloc(1, sizeof(Definitions));
	definitions->def = newDefinition;
	definitions->next = otherDefinitions;
	return definitions;
}

Definition * ConfigDefinitionSemanticAction(ConfigSentence * configDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->config = configDef;
	definition->type = CONFIG;
	return definition;
}

Definition * TrackDefinitionSemanticAction(Track * trackDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->track = trackDef;
	definition->type = TRACK_DEF;
	return definition;
}

Definition * PatternDefinitionSemanticAction(PatternDefinition * patternDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->pattern = patternDef;
	definition->type = PATTERN_DEF;
	return definition;
}

Play * PlayTracksSemanticAction(TrackIDs * tracks) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Play * play = calloc(1, sizeof(Play));
	play->tracks = tracks;
	play->type = TRACKS;
	return play;
}

Play * PlayAllSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Play * play = calloc(1, sizeof(Play));
	play->type = ALL;
	return play;
}

TrackIDs * TrackListSemanticAction(ID * newID, TrackIDs * otherIDs) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TrackIDs * tracks = calloc(1, sizeof(TrackIDs));
	tracks->id = newID;
	tracks->next = otherIDs;
	return tracks;
}

ConfigSentence * GlobalTempoSemanticAction(Tempo * tempo) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConfigSentence * sentence = calloc(1, sizeof(ConfigSentence));
	sentence->tempo = tempo;
	sentence->type = TEMPO;
	return sentence;
}

ConfigSentence * GlobalKeySemanticAction(Key * key) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConfigSentence * sentence = calloc(1, sizeof(ConfigSentence));
	sentence->key = key;
	sentence->type = KEY;
	return sentence;
}

Tempo * TempoSemanticAction(const float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Tempo * tempo = calloc(1, sizeof(Tempo));
	tempo->value = value;
	return tempo; // Are you rushing or dragging
}

Key * KeySemanticAction(NoteID * noteID, Mode * mode) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Key * key = calloc(1, sizeof(Key));
	key->note = noteID;
	key->mode = mode;
	return key;
}

NoteID * NoteIDSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoteID * note = calloc(1, sizeof(NoteID));
	note->value = value;
	return note;
}

Mode * ModeSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Mode * mode = calloc(1, sizeof(Mode));
	mode->value = value;
	return mode;
}

Track * TrackSemanticAction(Instrument * instrument, Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Track * track = calloc(1, sizeof(Track));
	track->instrument = instrument;
	track->sentences = sentences;
	return track;
}