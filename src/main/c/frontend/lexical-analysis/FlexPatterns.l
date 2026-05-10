%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "../../support/type/LexerKeywords.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus DurationLexemeAction(TokenLabel label);
extern CompilationStatus ModeLexemeAction(TokenLabel label);
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
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
digit               [0-9]

%%

"//".*                              { return IgnoredLexemeAction(); }

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

"whole"|"w"                         { return DurationLexemeAction(WHOLE); }
"half"|"h"                          { return DurationLexemeAction(HALF); }
"quarter"|"q"                       { return DurationLexemeAction(QUARTER); }
"eighth"|"e"                        { return DurationLexemeAction(EIGHTH); }
"sixteenth"|"s"                     { return DurationLexemeAction(SIXTEENTH); }
"thirtysecond"|"ths"                { return DurationLexemeAction(THIRTYSECOND); }
"sixtyfourth"|"sxf"                 { return DurationLexemeAction(SIXTYFOURTH); }

"major"|"ionian"                    { return ModeLexemeAction(MAJOR); }
"dorian"                            { return ModeLexemeAction(DORIAN); }
"phrygian"                          { return ModeLexemeAction(PHRYGIAN); }
"lydian"                            { return ModeLexemeAction(LYDIAN); }
"mixolydian"                        { return ModeLexemeAction(MIXOLYDIAN); }
"minor"|"aeolian"                   { return ModeLexemeAction(MINOR); }
"locrian"                           { return ModeLexemeAction(LOCRIAN); }

"all"                               { return KeywordLexemeAction(TOK_PLAY_ALL); }

{digit}+"."{digit}+                 { return LiteralFloatLexemeAction(); }
{digit}+                            { return LiteralIntegerLexemeAction(); }
[A-G](#|b)?                         { return LiteralNoteIdLexemeAction(); }
\"([^\\\"]|\\.)*\"                  { return LiteralStringLexemeAction(); }

"-"									{ return ArithmeticOperatorLexemeAction(TOK_SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(TOK_MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(TOK_DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(TOK_ADD); }
"dot"                               { return ArithmeticOperatorLexemeAction(TOK_DOT); }

"{"									{ return StructureOperatorLexemeAction(TOK_OPEN_BRACE); }
"}"									{ return StructureOperatorLexemeAction(TOK_CLOSE_BRACE); }
"("									{ return StructureOperatorLexemeAction(TOK_OPEN_PARENTHESIS); }
")"									{ return StructureOperatorLexemeAction(TOK_CLOSE_PARENTHESIS); }
"["									{ return StructureOperatorLexemeAction(TOK_OPEN_SQUARE); }
"]"									{ return StructureOperatorLexemeAction(TOK_CLOSE_SQUARE); }
","									{ return StructureOperatorLexemeAction(TOK_COMMA); }
";"									{ return StructureOperatorLexemeAction(TOK_SEMICOLON); }

[a-zH-Z_][a-zA-Z0-9_]*              { return NamingLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }
<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
