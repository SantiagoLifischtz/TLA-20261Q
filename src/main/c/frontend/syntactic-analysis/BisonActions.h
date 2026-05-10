#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState);

/**
 * Bison semantic actions.
 */

Number * IntegerSemanticAction(const int value);
Number * FloatSemanticAction(const float value);
Expression * BinaryOperationSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * UnaryOperationSemanticAction(Expression * operand, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Number * number);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Program * ProgramSemanticAction(Definitions * definitions, Play * playBlock);
Definitions * DefinitionListSemanticAction(Definition * newDefinition, Definitions * otherDefinitions);
Definition * ConfigDefinitionSemanticAction(ConfigSentence * configDef);
Definition * TrackDefinitionSemanticAction(Track * trackDef);
Definition * PatternDefinitionSemanticAction(PatternDefinition * patternDef);
PatternDefinition * PatternDeclarationSemanticAction(Identifier * id, Block * block);
Play * PlayTracksSemanticAction(TrackIDs * tracks);
Play * PlayAllSemanticAction();
TrackIDs * TrackListSemanticAction(Identifier * newID, TrackIDs * otherIDs);
ConfigSentence * GlobalTempoSemanticAction(Tempo * tempo);
ConfigSentence * GlobalKeySemanticAction(Key * key);
Tempo * TempoSemanticAction(Expression * exp);
Key * KeySemanticAction(NoteID * noteID, Mode * mode);
NoteID * NoteIDSemanticAction(const char value);
Mode * ModeSemanticAction(const char value);
Track * TrackSemanticAction(Identifier * id, Instrument * instrument, Sentences * sentences);
Instrument * InstrumentSemanticAction(char* name);
Sentences * SentenceListSemanticAction(Sentence * newSentence, Sentences * otherSentences);
Sentence * PatternSentenceSemanticAction(PatternSentence * ps);
Sentence * TempoSentenceSemanticAction(Tempo * tempo);
Sentence * KeySentenceSemanticAction(Key * key);
PatternSentence * InlinePSSemanticAction(InlinePattern * pattern, Wait * wait);
PatternSentence * BlockPSSemanticAction(Block * block, Wait * wait);
PatternSentence * RepeatPSSemanticAction(Repeat * repeat);
PatternSentence * StepPSSemanticAction(Step * step, Wait * wait);
Pattern * PatternSemanticAction(InlinePattern * inlinePattern);
Pattern * BlockPatternSemanticAction(Block * block);
Wait * WaitSemanticAction(Duration * duration);
InlinePattern * NotePatternSemanticAction(Note * note);
InlinePattern * RestPatternSemanticAction(Rest * rest);
InlinePattern * ChordPatternSemanticAction(Chord * chord);
InlinePattern * StrumPatternSemanticAction(Strum * strum);
InlinePattern * ArpeggioPatternSemanticAction(Arpeggio * arp);
InlinePattern * IDPatternSemanticAction(Identifier * id);
Note * NoteSemanticAction(NoteAndOctave * noteAndOctave, Duration * duration);
NoteAndOctave * NoteOctaveSemanticAction(NoteID * noteID, const int octave);
NoteAndOctave * DegreeNoteSemanticAction(Degree * degree);
NoteAndOctave * NoteFromStringSemanticAction(char* identifier);
Duration * DurationSemanticAction(Expression * exp);
Rest * RestSemanticAction(Duration * duration);
Repeat * RepeatSemanticAction(const int count, PatternSentence * ps);
Step * StepSemanticAction(Duration * interval, StepBlock * block);
Block * BlockSemanticAction(Sentences * sentences);
StepBlock * StepBlockSemanticAction(NoWaitSentences * nws);
NoWaitSentences * NoWaitSentencesSemanticAction(NoWaitSentence * sentence, NoWaitSentences * otherSentences);
NoWaitSentence * InlineNoWaitSemanticAction(InlinePattern * pattern);
NoWaitSentence * BlockNoWaitSemanticAction(Block * block);
Chord * ChordOfPatternsSemanticAction(PatternChord * pc);
Chord * ChordOfNotesSemanticAction(NoteChord * nc);
PatternChord * PatternChordSemanticAction(CommaSeparatedPatterns * csp);
NoteChord * NoteChordSemanticAction(CommaSeparatedNotes * csn);
CommaSeparatedPatterns * CSPSemanticAction(Pattern * pattern, CommaSeparatedPatterns * otherPatterns);
CommaSeparatedNotes * CSNSemanticAction(NoteAndOctave * note, CommaSeparatedNotes * otherNotes);
Strum * StrumSemanticAction(CommaSeparatedNotes * csn, Duration * total, Duration * interval);
Arpeggio * ArpeggioSemanticAction(CommaSeparatedNotes * csn, Duration * total);
Degree * DegreeSemanticAction(const int number, const int octave);
Identifier * IdentifierSemanticAction(char *name);

#endif
