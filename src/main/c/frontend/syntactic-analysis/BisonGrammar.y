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
%define api.token.prefix {TOK_}

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

/** Lexer-only keywords */
%token <token> WHOLE HALF QUARTER EIGHT SIXTEENTH THIRTYSECOND SIXTYFOURTH
%token <token> MAJOR DORIAN PHRYGIAN LYDIAN MIXOLYDIAN MINOR LOCRIAN

/* Extra. */
%token <token> IGNORED
%token <token> UNKNOWN

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
%token <token> PLAY_ALL
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

%token <token> DOT

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
%type <wait> optional_wait
%type <wait> wait_after_block
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
	| %empty												{ $$ = NULL; }
	;

definition: config_sentence									{ $$ = ConfigDefinitionSemanticAction($1); }
	| pattern_definition									{ $$ = PatternDefinitionSemanticAction($1); }
	| track													{ $$ = TrackDefinitionSemanticAction($1); }
	;

play: PLAY OPEN_BRACE track_ids[ids] SEMICOLON CLOSE_BRACE	{ $$ = PlayTracksSemanticAction($ids); }
	| PLAY PLAY_ALL SEMICOLON								{ $$ = PlayAllSemanticAction(); }
	;

track_ids: id												{ $$ = TrackListSemanticAction($1, NULL); }
	| id COMMA track_ids									{ $$ = TrackListSemanticAction($1, $3); }
	;

id: ID														{ $$ = IdentifierSemanticAction($1); }
	;

config_sentence: tempo										{ $$ = GlobalTempoSemanticAction($1); }
	| key													{ $$ = GlobalKeySemanticAction($1); }
	;

tempo: TEMPO expression SEMICOLON							{ $$ = TempoSemanticAction($2); }
	;

key: KEY note_id mode SEMICOLON								{ $$ = KeySemanticAction($2, $3); }
	;

note_id: NOTE_ID											{ $$ = NoteIDSemanticAction($1); }
	;

mode: MODE													{ $$ = ModeSemanticAction($1); }
	;

track: TRACK id[id] OPEN_BRACE instrument[inst] sentences[sent] CLOSE_BRACE	{ $$ = TrackSemanticAction($id, $inst, $sent); }
	;

pattern_definition: PATTERN id block						{ $$ = PatternDeclarationSemanticAction($2, $3); }
	;

instrument: INSTRUMENT STRING SEMICOLON						{ $$ = InstrumentSemanticAction($2); }
	;

sentences: sentence sentences								{ $$ = SentenceListSemanticAction($1, $2); }
	| %empty												{ $$ = NULL; }
	;

sentence: pattern_sentence									{ $$ = PatternSentenceSemanticAction($1); }
	| tempo													{ $$ = TempoSentenceSemanticAction($1); }
	| key													{ $$ = KeySentenceSemanticAction($1); }
	;

pattern_sentence: inline_pattern optional_wait SEMICOLON	{ $$ = InlinePSSemanticAction($1, $2); }
	| block wait_after_block								{ $$ = BlockPSSemanticAction($1, $2); }
	| repeat												{ $$ = RepeatPSSemanticAction($1); }
	| step wait_after_block									{ $$ = StepPSSemanticAction($1, $2); }
	;

pattern: inline_pattern										{ $$ = PatternSemanticAction($1); }
	| block													{ $$ = BlockPatternSemanticAction($1); }
	;

optional_wait: duration										{ $$ = WaitSemanticAction($1); }
	| %empty												{ $$ = NULL; }
	;

wait_after_block: duration SEMICOLON						{ $$ = WaitSemanticAction($1); }
	| %empty												{ $$ = NULL; }
	;

duration: expression										{ $$ = DurationSemanticAction($1); }
	;

inline_pattern: note										{ $$ = NotePatternSemanticAction($1); }
	| rest													{ $$ = RestPatternSemanticAction($1); }
	| chord													{ $$ = ChordPatternSemanticAction($1); }
	| strum													{ $$ = StrumPatternSemanticAction($1); }
	| arpeggio												{ $$ = ArpeggioPatternSemanticAction($1); }
	| degree												{ $$ = DegreePatternSemanticAction($1); }
	| id													{ $$ = IDPatternSemanticAction($1); }
	;

note: note_and_octave duration								{ $$ = NoteSemanticAction($1, $2); }
	;

note_and_octave: note_id INTEGER							{ $$ = NoteOctaveSemanticAction($1, $2); }
	;

rest: REST duration											{ $$ = RestSemanticAction($2); }
	;

repeat: REPEAT INTEGER pattern_sentence						{ $$ = RepeatSemanticAction($2, $3); }
	;

step: STEP duration step_block								{ $$ = StepSemanticAction($2, $3); }
	;

block: OPEN_BRACE sentences CLOSE_BRACE						{ $$ = BlockSemanticAction($2); }
	;

step_block: OPEN_BRACE no_wait_sentences CLOSE_BRACE		{ $$ = StepBlockSemanticAction($2); }
	;

no_wait_sentences: no_wait_sentence no_wait_sentences		{ $$ = NoWaitSentencesSemanticAction($1, $2); }
	| %empty												{ $$ = NULL; }
	;

no_wait_sentence: inline_pattern SEMICOLON					{ $$ = InlineNoWaitSemanticAction($1); }
	| block													{ $$ = BlockNoWaitSemanticAction($1); }
	;

chord: pattern_chord										{ $$ = ChordOfPatternsSemanticAction($1); }
	| note_chord											{ $$ = ChordOfNotesSemanticAction($1); }
	;

pattern_chord: OPEN_SQUARE comma_separated_patterns CLOSE_SQUARE	{ $$ = PatternChordSemanticAction($2); }
	;

note_chord: OPEN_SQUARE comma_separated_notes CLOSE_SQUARE	{ $$ = NoteChordSemanticAction($2); }
	;

comma_separated_patterns: pattern							{ $$ = CSPSemanticAction($1, NULL); }
	| pattern COMMA comma_separated_patterns				{ $$ = CSPSemanticAction($1, $3); }
	;

comma_separated_notes: note_and_octave						{ $$ = CSNSemanticAction($1, NULL); }
	| note_and_octave COMMA comma_separated_notes			{ $$ = CSNSemanticAction($1, $3); }
	;

strum: STRUM OPEN_PARENTHESIS comma_separated_notes[notes] COMMA duration[total] COMMA duration[interval] CLOSE_PARENTHESIS		{ $$ = StrumSemanticAction($notes, $total, $interval); }
	;

arpeggio: ARPEGGIO OPEN_PARENTHESIS comma_separated_notes[notes] COMMA duration[total] CLOSE_PARENTHESIS	{ $$ = ArpeggioSemanticAction($notes, $total); }
	;

degree: DEGREE OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS duration	{ $$ = DegreeSemanticAction($3, $5, $7); }
	;

expression: expression[left] ADD expression[right]			{ $$ = BinaryOperationSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]				{ $$ = BinaryOperationSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]				{ $$ = BinaryOperationSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]				{ $$ = BinaryOperationSemanticAction($left, $right, SUBTRACTION); }
	| expression DOT										{ $$ = UnaryOperationSemanticAction($1, DOT); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 		{ $$ = ExpressionFactorSemanticAction($2); }
	| number 												{ $$ = ConstantFactorSemanticAction($1); }
	;

number: INTEGER												{ $$ = IntegerSemanticAction($1); }
	| FLOAT													{ $$ = FloatSemanticAction($1); }
	| DURATION_BUILTIN										{ $$ = FloatSemanticAction($1); }
	;

%%
