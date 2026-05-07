%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

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
	float floatValue;
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
	TrackIDs * trackIDs;
	ConfigSentence * configSentence;
	Tempo * tempo;
	Key * key;
	Mode * mode;
	NoteID * noteID;
	Track * track;
	Instrument * instrument;
	Sentences * sentences;
	Sentence * sentence;
	PatternDefinition * patternDefinition;
	PatternSentence * patternSentence;
	Pattern * pattern;
	Wait * wait;
	InlinePattern * inlinePattern;
	Note * note;
	NoteAndOctave * noteAndOctave;
	Duration * duration;
	Rest * rest;
	Repeat * repeat;
	Step * step;
	StepBlock * stepBlock;
	NoWaitSentences * noWaitSentences;
	NoWaitSentence * noWaitSentence;
	Block * block;
	Chord * chord;
	PatternChord * patternChord;
	CommaSeparatedPatterns * commaSeparatedPatterns;
	NoteChord * noteChord;
	CommaSeparatedNotes * commaSeparatedNotes;
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
%destructor { destroyTrackIDs($$); } <trackIDs>
%destructor { destroyConfigSentence($$); } <configSentence>
%destructor { destroyTempo($$); } <tempo>
%destructor { destroyKey($$); } <key>
%destructor { destroyMode($$); } <mode>
%destructor { destroyNoteID($$); } <noteID>
%destructor { destroyTrack($$); } <track>
%destructor { destroyInstrument($$); } <instrument>
%destructor { destroySentences($$); } <sentences>
%destructor { destroySentence($$); } <sentence>
%destructor { destroyPatternDefinition($$); } <patternDefinition>
%destructor { destroyPatternSentence($$); } <patternSentence>
%destructor { destroyPattern($$); } <pattern>
%destructor { destroyWait($$); } <wait>
%destructor { destroyInlinePattern($$); } <inlinePattern>
%destructor { destroyNote($$); } <note>
%destructor { destroyNoteAndOctave($$); } <noteAndOctave>
%destructor { destroyDuration($$); } <duration>
%destructor { destroyRest($$); } <rest>
%destructor { destroyRepeat($$); } <repeat>
%destructor { destroyStep($$); } <step>
%destructor { destroyStepBlock($$); } <stepBlock>
%destructor { destroyNoWaitSentences($$); } <noWaitSentences>
%destructor { destroyNoWaitSentence($$); } <noWaitSentence>
%destructor { destroyBlock($$); } <block>
%destructor { destroyChord($$); } <chord>
%destructor { destroyPatternChord($$); } <patternChord>
%destructor { destroyCommaSeparatedPatterns($$); } <commaSeparatedPatterns>
%destructor { destroyNoteChord($$); } <noteChord>
%destructor { destroyCommaSeparatedNotes($$); } <commaSeparatedNotes>
%destructor { destroyStrum($$); } <strum>
%destructor { destroyArpeggio($$); } <arpeggio>
%destructor { destroyDegree($$); } <degree>
%destructor { destroyID($$); } <id>

/** Terminals. */
%token <integer> INTEGER
%token <floatValue> FLOAT
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
%token <floatValue> DURATION_BUILTIN

/** Non-terminals. */
%type <number> number
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression 										{ $$ = ExpressionProgramSemanticAction($1); }
	;

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
