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
typedef enum NumberType NumberType;

typedef enum DefinitionType DefinitionType;
typedef enum SentenceType SentenceType;
typedef enum PatternSentenceType PatternSentenceType;
typedef enum PatternType PatternType;
typedef enum InlinePatternType InlinePatternType;
typedef enum ChordType ChordType;
typedef enum PlayBlockType PlayBlockType;

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
typedef struct Tempo Tempo;
typedef struct Key Key;
typedef struct Mode Mode;
typedef struct NoteID NoteID;
typedef struct Track Track;
typedef struct Instrument Instrument;
typedef struct Sentences Sentences;
typedef struct Sentence Sentence;
typedef struct PatternDefinition PatternDefinition;
typedef struct PatternSentence PatternSentence;
typedef struct Pattern Pattern;
typedef struct Wait Wait;
typedef struct InlinePattern InlinePattern;
typedef struct Note Note;
typedef struct NoteAndOctave NoteAndOctave;
typedef struct Duration Duration;
typedef struct Rest Rest;
typedef struct Repeat Repeat;
typedef struct Step Step;
typedef struct StepBlock StepBlock;
typedef struct NoWaitSentences NoWaitSentences;
typedef struct NoWaitSentence NoWaitSentence;
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

enum SentenceType {
	PATTERN,
	KEY,
	TEMPO
};

enum DefinitionType {
	CONFIG,
	PATTERN_DEF,
	TRACK_DEF
};

enum PatternSentenceType {
	INLINE_SENTENCE,
	BLOCK_SENTENCE,
	REPEAT,
	STEP
};

enum PatternType {
	INLINE,
	BLOCK
};

enum InlinePatternType {
	NOTE,
	REST,
	CHORD,
	STRUM,
	ARPEGGIO,
	DEGREE,
	PATTERN_ID
};

enum NumberType {
	INTEGER,
	FLOAT
};

enum ChordType {
	PATTERN_CHORD,
	NOTE_CHORD
};

enum PlayBlockType {
	TRACKS,
	ALL
};

struct Constant {
	int value;
};

struct Number { // TODO: reemplazar constant por esto en todos lados
	union {
		int intValue;
		float floatValue;
	};
	NumberType type;
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
	Definitions * definitions;
	Play * playBlock;
};

struct Definitions {
	Definition * def;
	Definitions * next;
};

struct Definition {
	union {
		ConfigSentence * config;
		PatternDefinition * pattern;
		Track * track;
	};
	DefinitionType type;
};

struct Play {
	TrackIDs * tracks;
	PlayBlockType type; // Si esta en all, tracks es ignorado.
};

struct ID {
	char * name;
};

struct TrackIDs {
	ID * id;
	TrackIDs * next;
};

struct ConfigSentence {
	union {
		Key * key;
		Tempo * tempo;
	};
	SentenceType type;
};

struct Tempo {
	float value;
};

struct Key {
	NoteID * note;
	Mode * mode;
};

struct Mode {
	char value; // 0 - 6
};

struct NoteID {
	char value; // 0 - 11
};

struct Track {
	Instrument * instrument;
	Sentences * sentences;
};

struct Instrument {
	char *name;
};

struct Sentences {
	Sentence * sentence;
	Sentences * next;
};

struct Sentence {
	union {
		Pattern * pattern;
		Tempo * tempo;
		Key * key;
	};
	SentenceType type;
};

struct PatternDefinition {
	Block * block;
};

struct PatternSentence {
	union {
		struct {
			InlinePattern * inlinePattern;
			Wait * inlineWait;
		};
		struct {
			Block * block;
			Wait * blockWait;
		};
		Repeat * repeat;
		Step * step;
	};
	PatternSentenceType type;
};

struct Pattern {
	union {
		InlinePattern * inlinePattern;
		Block * block;
	};
	PatternType type;
};

struct Block {
	Sentences * sentences;
};

struct Wait {
	float value;
};

struct InlinePattern {
	union {
		Note * note;
		Rest * rest;
		Chord * chord;
		Strum * strum;
		Arpeggio * arpeggio;
		Degree * degree;
		ID * id;
	};
	InlinePatternType type;
};

struct Note {
	NoteAndOctave * noteAndOctave;
	Duration * duration;
};

struct NoteAndOctave {
	NoteID * note;
	char octave; // midi value = note->value + 12 * octave
};

struct Duration {
	Expression * expression;
};

struct Rest {
	Duration * duration;
};

struct Repeat {
	int count;
	PatternSentence sentence;
};

struct Step {
	Duration * interval;
	StepBlock * block;
};

struct StepBlock {
	NoWaitSentences * sentences;
};

struct NoWaitSentences {
	NoWaitSentence * sentence;
	NoWaitSentences * next;
};

struct NoWaitSentence {
	Pattern * pattern;
};

struct Chord {
	union {
		PatternChord * patternChord;
		NoteChord * noteChord;
	};
	ChordType type;
};

struct PatternChord {
	CommaSeparatedPatterns * patterns;
};

struct NoteChord {
	CommaSeparatedNotes * notes;
};

struct CommaSeparatedPatterns {
	Pattern * pattern;
	CommaSeparatedPatterns * next;
};

struct CommaSeparatedNotes {
	NoteAndOctave * note;
	CommaSeparatedNotes * next;
};

struct Strum {
	CommaSeparatedNotes * notes;
	Duration * totalDuration;
	Duration * interval;
};

struct Arpeggio {
	CommaSeparatedNotes * notes;
	Duration * totalDuration;
};

struct Degree {
	int number;
	int octave;
	Duration * duration;
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
