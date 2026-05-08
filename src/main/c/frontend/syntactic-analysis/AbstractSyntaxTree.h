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

typedef struct Number Number;
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
	SUBTRACTION,
	DOT
};

enum FactorType {
	NUMBER,
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

struct Number {
	union {
		int intValue;
		float floatValue;
	};
	NumberType type;
};

struct Factor {
	union {
		Number * number;
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
		Expression * singleExpression;
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
	Expression * expression;
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
		PatternSentence * patternSentence;
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
			union {
				InlinePattern * inlinePattern;
				Block * block;
			};
			Wait * wait;
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
	Duration * duration;
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
	PatternSentence * sentence;
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

void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyNumber(Number * number);
void destroyProgram(Program * program);
void destroyDefinitions(Definitions * definitions);
void destroyDefinition(Definition * definition);
void destroyPlay(Play * play);
void destroyID(ID * id);
void destroyTrackIDs(TrackIDs * trackIDs);
void destroyConfigSentence(ConfigSentence * configSentence);
void destroyTempo(Tempo * tempo);
void destroyKey(Key * key);
void destroyMode(Mode * mode);
void destroyNoteID(NoteID * noteID);
void destroyTrack(Track * track);
void destroyInstrument(Instrument * instrument);
void destroySentences(Sentences * sentences);
void destroySentence(Sentence * sentence);
void destroyPatternDefinition(PatternDefinition * patternDefinition);
void destroyPatternSentence(PatternSentence * patternSentence);
void destroyPattern(Pattern * pattern);
void destroyBlock(Block * block);
void destroyWait(Wait * wait);
void destroyInlinePattern(InlinePattern * inlinePattern);
void destroyNote(Note * note);
void destroyNoteAndOctave(NoteAndOctave * noteAndOctave);
void destroyDuration(Duration * duration);
void destroyRest(Rest * rest);
void destroyRepeat(Repeat * repeat);
void destroyStep(Step * step);
void destroyStepBlock(StepBlock * stepBlock);
void destroyNoWaitSentences(NoWaitSentences * noWaitSentences);
void destroyNoWaitSentence(NoWaitSentence * noWaitSentence);
void destroyChord(Chord * chord);
void destroyPatternChord(PatternChord * patternChord);
void destroyNoteChord(NoteChord * noteChord);
void destroyCommaSeparatedPatterns(CommaSeparatedPatterns * commaSeparatedPatterns);
void destroyCommaSeparatedNotes(CommaSeparatedNotes * commaSeparatedNotes);
void destroyStrum(Strum * strum);
void destroyArpeggio(Arpeggio * arpeggio);
void destroyDegree(Degree * degree);

#endif
