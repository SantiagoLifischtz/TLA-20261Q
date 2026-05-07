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
extern CompilationStatus LiteralNoteLexemeAction();
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

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

// Keywords {}
"pattern"                           { return KeywordLexemeAction(PATTERN); }
"repeat"                            { return KeywordLexemeAction(REPEAT); }
"step"                              { return KeywordLexemeAction(STEP); }
"track"                             { return KeywordLexemeAction(TRACK); }
"play"                              { return KeywordLexemeAction(PLAY); }

// Keywords line
"instrument"                        { return KeywordLexemeAction(INSTRUMENT); }
"tempo"                             { return KeywordLexemeAction(TEMPO); }
"key"                               { return KeywordLexemeAction(KEY); }
"rest"                              { return KeywordLexemeAction(REST); }

// Keywords ()
"strum"                             { return KeywordLexemeAction(STRUM); }
"arpeggio"                          { return KeywordLexemeAction(ARPEGGIO); }
"degree"                            { return KeywordLexemeAction(DEGREE); }

// Keywords durations
"dot"                               { return KeywordLexemeAction(DOT); }
"whole"|"w"                         { return KeywordLexemeAction(WHOLE); }
"half"|"h"                          { return KeywordLexemeAction(HALF); }
"quarter"|"q"                       { return KeywordLexemeAction(QUARTER); }
"eighth"|"e"                        { return KeywordLexemeAction(EIGHT); }
"sixteenth"|"s"                     { return KeywordLexemeAction(SIXTEENTH); }
"thirtysecond"|"ths"                { return KeywordLexemeAction(THIRTYSECOND); }
"sixtyfourth"|"sxf"                 { return KeywordLexemeAction(SIXTYFOURTH); }

// Keywords modes
"major"|"ionian"                    { return KeywordLexemeAction(MAJOR); }
"dorian"                            { return KeywordLexemeAction(DORIAN); }
"phrygian"                          { return KeywordLexemeAction(PHRYGIAN); }
"lydian"                            { return KeywordLexemeAction(LYDIAN); }
"mixolydian"                        { return KeywordLexemeAction(MIXOLYDIAN); }
"minor"|"aeolian"                   { return KeywordLexemeAction(MINOR); }
"locrian"                           { return KeywordLexemeAction(LOCRIAN); }

// Keywords special
"all"                               { return KeywordLexemeAction(ALL); }

// Literals
{digit}+"."{digit}+                 { return LiteralFloatLexemeAction(); }
{digit}+                            { return LiteralIntegerLexemeAction(); }
[A-G](#|b)?[0-9]+                   { return LiteralNoteLexemeAction(); }
\"([^\\\"]|\\.)*\"                  { return LiteralStringLexemeAction(); }

// Identifier (naming and percussion)
[a-zA-Z_][a-zA-Z0-9_]*              { return NamingLexemeAction(); } // TODO check, could be the same as LiteralNote

// Arithmetic
"-"									{ return ArithmeticOperatorLexemeAction(SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(ADD); }

// Structure
"{"									{ return StructureOperatorLexemeAction(OPEN_BRACE); }
"}"									{ return StructureOperatorLexemeAction(CLOSE_BRACE); }
"("									{ return StructureOperatorLexemeAction(OPEN_PARENTHESIS); }
")"									{ return StructureOperatorLexemeAction(CLOSE_PARENTHESIS); }
"["									{ return StructureOperatorLexemeAction(OPEN_BRACKET); }
"]"									{ return StructureOperatorLexemeAction(CLOSE_BRACKET); }
","									{ return StructureOperatorLexemeAction(COMMA); }
";"									{ return StructureOperatorLexemeAction(SEMICOLON); }

// Comment
"//".*                              { return IgnoredLexemeAction(); }

// Special
{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }
<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
