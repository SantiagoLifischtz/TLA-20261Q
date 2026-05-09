%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus LiteralFloatLexemeAction();
extern CompilationStatus LiteralIntegerLexemeAction();
extern CompilationStatus LiteralNoteIdLexemeAction();
extern CompilationStatus LiteralStringLexemeAction();
extern CompilationStatus NamingLexemeAction();
extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus StructureOperatorLexemeAction(TokenLabel label);
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus UnknownLexemeAction();
extern CompilationStatus EOFLexemeAction();

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x IMPORT_EXPRESSION
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
digit               [0-9]

%%

"pattern"                           { return KeywordLexemeAction(TOK_PATTERN); }
"repeat"                            { return KeywordLexemeAction(TOK_REPEAT); }
"step"                              { return KeywordLexemeAction(TOK_STEP); }
"track"                             { return KeywordLexemeAction(TOK_TRACK); }
"play"                              { return KeywordLexemeAction(TOK_PLAY); }

"instrument"                        { return KeywordLexemeAction(TOK_INSTRUMENT); }
"tempo"                             { return KeywordLexemeAction(TOK_TEMPO); }
"key"                               { return KeywordLexemeAction(TOK_KEY); }
"rest"                              { return KeywordLexemeAction(TOK_REST); }

"strum"                             { return KeywordLexemeAction(TOK_STRUM); }
"arpeggio"                          { return KeywordLexemeAction(TOK_ARPEGGIO); }
"degree"                            { return KeywordLexemeAction(TOK_DEGREE); }

"dot"                               { return KeywordLexemeAction(TOK_DOTTED); }
"whole"|"w"                         { return KeywordLexemeAction(TOK_WHOLE); }
"half"|"h"                          { return KeywordLexemeAction(TOK_HALF); }
"quarter"|"q"                       { return KeywordLexemeAction(TOK_QUARTER); }
"eighth"|"e"                        { return KeywordLexemeAction(TOK_EIGHT); }
"sixteenth"|"s"                     { return KeywordLexemeAction(TOK_SIXTEENTH); }
"thirtysecond"|"ths"                { return KeywordLexemeAction(TOK_THIRTYSECOND); }
"sixtyfourth"|"sxf"                 { return KeywordLexemeAction(TOK_SIXTYFOURTH); }

"major"|"ionian"                    { return KeywordLexemeAction(TOK_MAJOR); }
"dorian"                            { return KeywordLexemeAction(TOK_DORIAN); }
"phrygian"                          { return KeywordLexemeAction(TOK_PHRYGIAN); }
"lydian"                            { return KeywordLexemeAction(TOK_LYDIAN); }
"mixolydian"                        { return KeywordLexemeAction(TOK_MIXOLYDIAN); }
"minor"|"aeolian"                   { return KeywordLexemeAction(TOK_MINOR); }
"locrian"                           { return KeywordLexemeAction(TOK_LOCRIAN); }

"all"                               { return KeywordLexemeAction(TOK_ALL); }

{digit}+"."{digit}+                 { return LiteralFloatLexemeAction(); }
{digit}+                            { return LiteralIntegerLexemeAction(); }
[A-G](#|b)?                         { return LiteralNoteIdLexemeAction(); }
\"([^\\\"]|\\.)*\"                  { return LiteralStringLexemeAction(); }

[a-zA-Z_][a-zA-Z0-9_]*              { return NamingLexemeAction(); }

"-"									{ return ArithmeticOperatorLexemeAction(TOK_SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(TOK_MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(TOK_DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(TOK_ADD); }

"{"									{ return StructureOperatorLexemeAction(TOK_OPEN_BRACE); }
"}"									{ return StructureOperatorLexemeAction(TOK_CLOSE_BRACE); }
"("									{ return StructureOperatorLexemeAction(TOK_OPEN_PARENTHESIS); }
")"									{ return StructureOperatorLexemeAction(TOK_CLOSE_PARENTHESIS); }
"["									{ return StructureOperatorLexemeAction(TOK_OPEN_SQUARE); }
"]"									{ return StructureOperatorLexemeAction(TOK_CLOSE_SQUARE); }
","									{ return StructureOperatorLexemeAction(TOK_COMMA); }
";"									{ return StructureOperatorLexemeAction(TOK_SEMICOLON); }

"//".*                              { return IgnoredLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }
<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
