#include "FlexActions.h"
#include "../syntactic-analysis/BisonParser.h"

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static InputBuffer * _inputBuffer = NULL;
static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownFlexActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: FlexActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	if (_inputBuffer != NULL) {
		destroyInputBuffer(_inputBuffer);
		_inputBuffer = NULL;
	}
	_lexicalAnalyzer = NULL;
}

ModuleDestructor initializeFlexActionsModule(LexicalAnalyzer * lexicalAnalyzer) {
	_inputBuffer = NULL;
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("FlexActions");
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	return _shutdownFlexActionsModule;
}

/* PRIVATE FUNCTIONS */

static void _logTokenAction(const char * actionName, Token * token);
static int _noteSemitone(const char * note);
static float _durationValue(TokenLabel label);
static int _modeValue(TokenLabel label);


/**
 * Logs a lexical-analyzer action over a token in DEBUGGING level.
 */
static void _logTokenAction(const char * actionName, Token * token) {
	char * _lexeme = escape(token->lexeme);
	logDebugging(_logger, WARNING_COLOR "%s" DEFAULT_COLOR ": Token(context=%d, label=%d, length=%d, lexeme=%s\"%s\"%s, line=%d, semanticValue=%p)",
		actionName,
		token->context,
		token->label,
		token->length,
		INFORMATION_COLOR, _lexeme, DEFAULT_COLOR,
		token->line,
		token->semanticValue);
	free(_lexeme);
	_lexeme = NULL;
}

static int _noteSemitone(const char * note) {
	switch (note[0]) {
		case 'C': return (note[1] == '#') ? 1 : (note[1] == 'b') ? 11 : 0;
		case 'D': return (note[1] == '#') ? 3 : (note[1] == 'b') ? 1 : 2;
		case 'E': return (note[1] == 'b') ? 3 : 4;
		case 'F': return (note[1] == '#') ? 6 : 5;
		case 'G': return (note[1] == '#') ? 8 : (note[1] == 'b') ? 6 : 7;
		case 'A': return (note[1] == '#') ? 10 : (note[1] == 'b') ? 8 : 9;
		case 'B': return (note[1] == 'b') ? 10 : 11;
	}

	return -1;
}

static float _durationValue(TokenLabel label) {
	switch (label) {
		case TOK_WHOLE:
			return 1.0;

		case TOK_HALF:
			return 0.5;

		case TOK_QUARTER:
			return 0.25;

		case TOK_EIGHT:
			return 0.125;

		case TOK_SIXTEENTH:
			return 0.0625;

		case TOK_THIRTYSECOND:
			return 0.03125;

		case TOK_SIXTYFOURTH:
			return 0.015625;

		default:
			return -1.0;
	}
}

static int _modeValue(TokenLabel label) {
	switch (label) {
		case TOK_MAJOR:
			return 0;
		
		case TOK_DORIAN:
			return 1;

		case TOK_PHRYGIAN:
			return 2;

		case TOK_LYDIAN:
			return 3;

		case TOK_MIXOLYDIAN:
			return 4;

		case TOK_MINOR:
			return 5;

		case TOK_LOCRIAN:
			return 6;

		default:
			return -1;
	}
}

/* PUBLIC FUNCTIONS */

CompilationStatus KeywordLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus DurationLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, TOK_DURATION_BUILTIN);
	token->semanticValue->float_value = _durationValue(label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ModeLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, TOK_MODE);
	token->semanticValue->integer = _modeValue(label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LiteralFloatLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_FLOAT);
	token->semanticValue->float_value = atof(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LiteralIntegerLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_INTEGER);
	token->semanticValue->integer = atoi(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LiteralNoteIdLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_NOTE_ID);
	token->semanticValue->integer = _noteSemitone(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LiteralStringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_STRING);
	size_t length = strlen(token->lexeme);
	char * value = malloc(length - 1);
	strncpy(value, token->lexeme + 1, length - 2);
	value[length - 2] = '\0';
	token->semanticValue->string = value;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus NamingLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_STRING);
	token->semanticValue->string = strdup(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus StructureOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IgnoredLexemeAction() {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, TOK_IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TOK_UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	destroyToken(token);
	return FAILED;
}

CompilationStatus EOFLexemeAction() {
	CompilationStatus status = IN_PROGRESS;
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	if (!popInputBuffer(_lexicalAnalyzer)) {
		status = pushToken(_lexicalAnalyzer, token);
		FlexContext context = currentLexicalAnalyzerContext(_lexicalAnalyzer);
		if (0 < context) {
			logError(_logger, "The final context is not closed (context=%d).", context);
			status = FAILED;
		}
	}
	destroyToken(token);
	return status;
}
