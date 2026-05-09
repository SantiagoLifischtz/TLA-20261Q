%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include <stdlib.h>

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

%}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

%union {
	/** Terminals. */

	signed int integer;
	float float_value;
	TokenLabel token;
	char * string;

	/** Non-terminals. */

	Number * number;
	Expression * expression;
	Factor * factor;
	Program * program;
	Definitions * definitions;
	Definition * definition;
	Play * play;
	TrackIDs * track_ids;
	ConfigSentence * config_sentence;
	Tempo * tempo;
	Key * key;
	Mode * mode;
	NoteID * note_id;
	Track * track;
	Instrument * instrument;
	Sentences * sentences;
	Sentence * sentence;
	PatternDefinition * pattern_definition;
	PatternSentence * pattern_sentence;
	Pattern * pattern;
	Wait * wait;
	InlinePattern * inline_pattern;
	Note * note;
	NoteAndOctave * note_and_octave;
	Duration * duration;
	Rest * rest;
	Repeat * repeat;
	Step * step;
	StepBlock * step_block;
	NoWaitSentences * no_wait_sentences;
	NoWaitSentence * no_wait_sentence;
	Block * block;
	Chord * chord;
	PatternChord * pattern_chord;
	CommaSeparatedPatterns * comma_separated_patterns;
	NoteChord * note_chord;
	CommaSeparatedNotes * comma_separated_notes;
	Strum * strum;
	Arpeggio * arpeggio;
	Degree * degree;
	ID * id;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { destroyNumber($$); } <number>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>
%destructor { destroyProgram($$); } <program>
%destructor { destroyDefinitions($$); } <definitions>
%destructor { destroyDefinition($$); } <definition>
%destructor { destroyPlay($$); } <play>
%destructor { destroyTrackIDs($$); } <track_ids>
%destructor { destroyConfigSentence($$); } <config_sentence>
%destructor { destroyTempo($$); } <tempo>
%destructor { destroyKey($$); } <key>
%destructor { destroyMode($$); } <mode>
%destructor { destroyNoteID($$); } <note_id>
%destructor { destroyTrack($$); } <track>
%destructor { destroyInstrument($$); } <instrument>
%destructor { destroySentences($$); } <sentences>
%destructor { destroySentence($$); } <sentence>
%destructor { destroyPatternDefinition($$); } <pattern_definition>
%destructor { destroyPatternSentence($$); } <pattern_sentence>
%destructor { destroyPattern($$); } <pattern>
%destructor { destroyWait($$); } <wait>
%destructor { destroyInlinePattern($$); } <inline_pattern>
%destructor { destroyNote($$); } <note>
%destructor { destroyNoteAndOctave($$); } <note_and_octave>
%destructor { destroyDuration($$); } <duration>
%destructor { destroyRest($$); } <rest>
%destructor { destroyRepeat($$); } <repeat>
%destructor { destroyStep($$); } <step>
%destructor { destroyStepBlock($$); } <step_block>
%destructor { destroyNoWaitSentences($$); } <no_wait_sentences>
%destructor { destroyNoWaitSentence($$); } <no_wait_sentence>
%destructor { destroyBlock($$); } <block>
%destructor { destroyChord($$); } <chord>
%destructor { destroyPatternChord($$); } <pattern_chord>
%destructor { destroyCommaSeparatedPatterns($$); } <comma_separated_patterns>
%destructor { destroyNoteChord($$); } <note_chord>
%destructor { destroyCommaSeparatedNotes($$); } <comma_separated_notes>
%destructor { destroyStrum($$); } <strum>
%destructor { destroyArpeggio($$); } <arpeggio>
%destructor { destroyDegree($$); } <degree>
%destructor { destroyID($$); } <id>

/** Terminals. */
%token <integer> INTEGER
%token <float_value> FLOAT
%token <string> STRING

%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

%token <token> SEMICOLON
%token <token> COMMA
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_SQUARE
%token <token> CLOSE_SQUARE
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> PLAY
%token <token> ALL
%token <token> TEMPO
%token <token> KEY
%token <token> TRACK
%token <token> INSTRUMENT
%token <string> ID
%token <token> PATTERN

%token <token> REST
%token <token> REPEAT
%token <token> STEP
%token <token> STRUM
%token <token> ARPEGGIO
%token <token> DEGREE

%token <integer> NOTE_ID

%token <token> DOTTED

%token <integer> MODE
%token <float_value> DURATION_BUILTIN

/** Non-terminals. */
%type <number> number
%type <expression> expression
%type <factor> factor
%type <program> program
%type <definitions> definitions
%type <definition> definition
%type <play> play
%type <track_ids> track_ids
%type <config_sentence> config_sentence
%type <tempo> tempo
%type <key> key
%type <mode> mode
%type <note_id> note_id
%type <track> track
%type <instrument> instrument
%type <sentences> sentences
%type <sentence> sentence
%type <pattern_definition> pattern_definition
%type <pattern_sentence> pattern_sentence
%type <pattern> pattern
%type <wait> wait
%type <inline_pattern> inline_pattern
%type <note> note
%type <note_and_octave> note_and_octave
%type <duration> duration
%type <rest> rest
%type <repeat> repeat
%type <step> step
%type <step_block> step_block
%type <no_wait_sentences> no_wait_sentences
%type <no_wait_sentence> no_wait_sentence
%type <block> block
%type <chord> chord
%type <pattern_chord> pattern_chord
%type <comma_separated_patterns> comma_separated_patterns
%type <note_chord> note_chord
%type <comma_separated_notes> comma_separated_notes
%type <strum> strum
%type <arpeggio> arpeggio
%type <degree> degree
%type <id> id

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

program: definitions play 									{ $$ = ProgramSemanticAction($1, $2); }
	;

definitions: definition definitions							{ $$ = DefinitionListSemanticAction($1, $2); }
	| %empty												{ $$ = NULL }
	;

definition: config_sentence

expression: expression[left] ADD expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 		{ $$ = ExpressionFactorSemanticAction($2); }
	| number 												{ $$ = ConstantFactorSemanticAction($1); }
	;

number: INTEGER												{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
