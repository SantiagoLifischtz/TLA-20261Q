#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;

typedef enum ConfigSentenceType ConfigSentenceType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct ID ID;

typedef struct Program Program;
typedef struct Definitions Definitions;
typedef struct Definition Definition;
typedef struct Play Play;
typedef struct TrackIDs TrackIDs;
typedef struct ConfigSentence ConfigSentence;
typedef struct ConfigSentenceContent ConfigSentenceContent;
typedef struct Tempo Tempo;
typedef struct Key Key;
typedef struct Mode Mode;
typedef struct Pitch Pitch;
typedef struct NoteLetter NoteLetter;
typedef struct SemitoneModifier;
typedef struct Track Track;
typedef struct Instrument Instrument;
typedef struct Sentences Sentences;
typedef struct Sentence Sentence;
typedef struct PatternDefinition PatternDefinition;
typedef struct Pattern Pattern;
typedef struct WaitAfterInline WaitAfterInline;
typedef struct WaitAfterBlock WaitAfterBlock;
typedef struct InlinePattern InlinePattern;
typedef struct Note Note;
typedef struct NoteID NoteID;
typedef struct Duration Duration;
typedef struct Rest Rest;
typedef struct Repeat Repeat;
typedef struct Step Step;
typedef struct Block Block;
typedef struct Chord Chord;
typedef struct PatternChord PatternChord;
typedef struct CommaSeparatedPatterns CommaSeparatedPatterns;
typedef struct NoteChord NoteChord;
typedef struct CommaSeparatedNotes CommaSeparatedNotes;
typedef struct Strum Strum;
typedef struct Arpeggio Arpeggio;
typedef struct Degree Degree;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
};

enum FactorType {
	CONSTANT,
	EXPRESSION
};

enum ConfigSentenceType {
	KEY,
	TEMPO
};

struct Constant {
	int value;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
};

struct Program {
	Expression * expression;
};

struct Definitions {
	Definition * def;
	Definitions * otherDefs;
};

struct Play {
	TrackIDs * tracks;
};

struct ID {
	char * name;
};

struct TrackIDs {
	ID * id;
	TrackIDs * ids;
};

struct ConfigSentence {
	ConfigSentenceContent * content;
};

struct ConfigSentenceContent {
	union {
		Key * key;
		Tempo * tempo;
	};
	ConfigSentenceType type;
};

struct Tempo {
	float value;
};

struct Key {
	Pitch * pitch;
	Mode * mode;
};

struct Mode {
	char value; // 0 - 6
};

struct Pitch {
	int noteNumber;
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyConstant(Constant * constant);
void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyProgram(Program * program);

// TODO add destroy functions

#endif
