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
	float float;
	TokenLabel token;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { destroyConstant($$); } <constant>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>

/** Terminals. */
%token <integer> INTEGER
%token <float> FLOAT
%token <token> STRING

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
%token <token> ID
%token <token> PATTERN

%token <token> REST
%token <token> REPEAT
%token <token> STEP
%token <token> STRUM
%token <token> ARPEGGIO
%token <token> DEGREE

%token <token> A
%token <token> B
%token <token> C
%token <token> D
%token <token> E
%token <token> F
%token <token> G
%token <token> SHARP
%token <token> FLAT
%token <token> NATURAL

%token <token> IONIAN
%token <token> MAJOR
%token <token> DORIAN
%token <token> PHRYGIAN
%token <token> LYDIAN
%token <token> MIXOLYDIAN
%token <token> AEOLIAN
%token <token> MINOR
%token <token> LOCRIAN

%token <token> DOTTED
%token <token> WHOLE
%token <token> HALF
%token <token> QUARTER
%token <token> EIGHTH
%token <token> SIXTEENTH
%token <token> THIRTYSECOND
%token <token> SIXTYFOURTH

/** Non-terminals. */
%type <constant> constant
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

program: expression											{ $$ = ExpressionProgramSemanticAction($1); }
	;

expression: expression[left] ADD expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS		{ $$ = ExpressionFactorSemanticAction($2); }
	| constant												{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER | FLOAT											{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
