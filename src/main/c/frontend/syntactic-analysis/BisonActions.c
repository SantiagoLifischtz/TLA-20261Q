#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Number * IntegerSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Number * number = calloc(1, sizeof(Number));
	number->intValue = value;
	number->type = INTEGER;
	return number;
}

Number * FloatSemanticAction(const float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Number * number = calloc(1, sizeof(Number));
	number->floatValue = value;
	number->type = FLOAT;
	return number;
}

Expression * BinaryOperationSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * UnaryOperationSemanticAction(Expression * operand, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->singleExpression = operand;
	expression->type = type;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Number * number) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->number = number;
	factor->type = NUMBER;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

Program * ProgramSemanticAction(Definitions * definitions, Play * playBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->definitions = definitions;
	program->playBlock = playBlock;
	return program;
}

Definitions * DefinitionListSemanticAction(Definition * newDefinition, Definitions * otherDefinitions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definitions * definitions = calloc(1, sizeof(Definitions));
	definitions->def = newDefinition;
	definitions->next = otherDefinitions;
	return definitions;
}

Definition * ConfigDefinitionSemanticAction(ConfigSentence * configDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->config = configDef;
	definition->type = CONFIG;
	return definition;
}

Definition * TrackDefinitionSemanticAction(Track * trackDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->track = trackDef;
	definition->type = TRACK_DEF;
	return definition;
}

Definition * PatternDefinitionSemanticAction(PatternDefinition * patternDef) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition * definition = calloc(1, sizeof(Definition));
	definition->pattern = patternDef;
	definition->type = PATTERN_DEF;
	return definition;
}

PatternDefinition * PatternDeclarationSemanticAction(ID * id, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternDefinition * def = calloc(1, sizeof(PatternDefinition));
	def->block = block;
	return def;
}

Play * PlayTracksSemanticAction(TrackIDs * tracks) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Play * play = calloc(1, sizeof(Play));
	play->tracks = tracks;
	play->type = TRACKS;
	return play;
}

Play * PlayAllSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Play * play = calloc(1, sizeof(Play));
	play->type = ALL;
	return play;
}

TrackIDs * TrackListSemanticAction(ID * newID, TrackIDs * otherIDs) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TrackIDs * tracks = calloc(1, sizeof(TrackIDs));
	tracks->id = newID;
	tracks->next = otherIDs;
	return tracks;
}

ConfigSentence * GlobalTempoSemanticAction(Tempo * tempo) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConfigSentence * sentence = calloc(1, sizeof(ConfigSentence));
	sentence->tempo = tempo;
	sentence->type = TEMPO;
	return sentence;
}

ConfigSentence * GlobalKeySemanticAction(Key * key) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConfigSentence * sentence = calloc(1, sizeof(ConfigSentence));
	sentence->key = key;
	sentence->type = KEY;
	return sentence;
}

Tempo * TempoSemanticAction(Expression * exp) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Tempo * tempo = calloc(1, sizeof(Tempo));
	tempo->expression = exp;
	return tempo; // Are you rushing or dragging
}

Key * KeySemanticAction(NoteID * noteID, Mode * mode) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Key * key = calloc(1, sizeof(Key));
	key->note = noteID;
	key->mode = mode;
	return key;
}

NoteID * NoteIDSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoteID * note = calloc(1, sizeof(NoteID));
	note->value = value;
	return note;
}

Mode * ModeSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Mode * mode = calloc(1, sizeof(Mode));
	mode->value = value;
	return mode;
}

Track * TrackSemanticAction(ID * id, Instrument * instrument, Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Track * track = calloc(1, sizeof(Track));
	track->instrument = instrument;
	track->sentences = sentences;
	return track;
}

Instrument * InstrumentSemanticAction(const char* name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Instrument * instrument = calloc(1, sizeof(Instrument));
	int length = strlen(name);
	instrument->name = malloc((length+1)*sizeof(char));
	strcpy(instrument->name, name);
	return instrument;
}

Sentences * SentenceListSemanticAction(Sentence * newSentence, Sentences * otherSentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentences * sentences = calloc(1, sizeof(Sentences));
	sentences->sentence = newSentence;
	sentences->next = otherSentences;
	return sentences;
}

Sentence * PatternSentenceSemanticAction(PatternSentence * ps) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->patternSentence = ps;
	sentence->type = PATTERN;
	return sentence;
}

Sentence * TempoSentenceSemanticAction(Tempo * tempo) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->tempo = tempo;
	sentence->type = TEMPO;
	return sentence;
}

Sentence * KeySentenceSemanticAction(Key * key) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->key = key;
	sentence->type = KEY;
	return sentence;
}

PatternSentence * InlinePSSemanticAction(InlinePattern * pattern, Wait * wait) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternSentence * ps = calloc(1, sizeof(PatternSentence));
	ps->inlinePattern = pattern;
	ps->wait = wait;
	ps->type = INLINE_SENTENCE;
	return ps;
}

PatternSentence * BlockPSSemanticAction(Block * block, Wait * wait) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternSentence * ps = calloc(1, sizeof(PatternSentence));
	ps->block = block;
	ps->wait = wait;
	ps->type = BLOCK_SENTENCE;
	return ps;
}

PatternSentence * RepeatPSSemanticAction(Repeat * repeat) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternSentence * ps = calloc(1, sizeof(PatternSentence));
	ps->repeat = repeat;
	ps->type = REPEAT;
	return ps;
}

PatternSentence * StepPSSemanticAction(Step * step, Wait * wait) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternSentence * ps = calloc(1, sizeof(PatternSentence));
	ps->step = step;
	ps->wait = wait;
	ps->type = STEP;
	return ps;
}

Pattern * PatternSemanticAction(InlinePattern * inlinePattern) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Pattern * p = calloc(1, sizeof(Pattern));
	p->inlinePattern = inlinePattern;
	p->type = INLINE;
	return p;
}

Pattern * BlockPatternSemanticAction(Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Pattern * p = calloc(1, sizeof(Pattern));
	p->block = block;
	p->type = BLOCK;
	return p;
}

Wait * WaitSemanticAction(Duration * duration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Wait * wait = calloc(1, sizeof(Wait));
	wait->duration = duration;
	return wait;
}

Wait * MissingWaitSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Number * zero = IntegerSemanticAction(0);
	Factor * factor = ConstantFactorSemanticAction(zero);
	Expression * expression = FactorExpressionSemanticAction(factor);
	Duration * duration = DurationSemanticAction(expression);
	return WaitSemanticAction(duration);
}

InlinePattern * NotePatternSemanticAction(Note * note) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->note = note;
	ip->type = NOTE;
	return ip;
}

InlinePattern * RestPatternSemanticAction(Rest * rest) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->rest = rest;
	ip->type = REST;
	return ip;
}

InlinePattern * ChordPatternSemanticAction(Chord * chord) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->chord = chord;
	ip->type = CHORD;
	return ip;
}

InlinePattern * StrumPatternSemanticAction(Strum * strum) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->strum = strum;
	ip->type = STRUM;
	return ip;
}

InlinePattern * ArpeggioPatternSemanticAction(Arpeggio * arp) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->arpeggio = arp;
	ip->type = ARPEGGIO;
	return ip;
}

InlinePattern * DegreePatternSemanticAction(Degree * degree) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->degree = degree;
	ip->type = DEGREE;
	return ip;
}

InlinePattern * IDPatternSemanticAction(ID * id) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InlinePattern * ip = calloc(1, sizeof(InlinePattern));
	ip->id = id;
	ip->type = PATTERN_ID;
	return ip;
}

Note * NoteSemanticAction(NoteAndOctave * noteAndOctave, Duration * duration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Note * note = calloc(1, sizeof(Note));
	note->noteAndOctave = noteAndOctave;
	note->duration = duration;
	return note;
}

NoteAndOctave * NoteOctaveSemanticAction(NoteID * noteID, const int octave) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoteAndOctave * n = calloc(1, sizeof(NoteAndOctave));
	n->note = noteID;
	n->octave = octave;
	return n;
}

Duration * DurationSemanticAction(Expression * exp) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Duration * d = calloc(1, sizeof(Duration));
	d->expression = exp;
	return d;
}

Rest * RestSemanticAction(Duration * duration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Rest * rest = calloc(1, sizeof(Rest));
	rest->duration = duration;
	return rest;
}

Repeat * RepeatSemanticAction(const int count, PatternSentence * ps) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Repeat * repeat = calloc(1, sizeof(Repeat));
	repeat->count = count;
	repeat->sentence = ps;
	return repeat;
}

Step * StepSemanticAction(Duration * interval, StepBlock * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Step * step = calloc(1, sizeof(Step));
	step->interval = interval;
	step->block = block;
	return step;
}

Block * BlockSemanticAction(Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * block = calloc(1, sizeof(Block));
	block->sentences = sentences;
	return block;
}

StepBlock * StepBlockSemanticAction(NoWaitSentences * nws) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StepBlock * stepBlock = calloc(1, sizeof(StepBlock));
	stepBlock->sentences = nws;
	return stepBlock;
}

NoWaitSentences * NoWaitSentencesSemanticAction(NoWaitSentence * sentence, NoWaitSentences * otherSentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoWaitSentences * nws = calloc(1, sizeof(NoWaitSentences));
	nws->sentence = sentence;
	nws->next = otherSentences;
	return nws;
}

NoWaitSentence * InlineNoWaitSemanticAction(InlinePattern * pattern) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoWaitSentence * sentence = calloc(1, sizeof(NoWaitSentence));
	sentence->inlinePattern = pattern;
	sentence->type = INLINE;
	return sentence;
}

NoWaitSentence * BlockNoWaitSemanticAction(Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoWaitSentence * sentence = calloc(1, sizeof(NoWaitSentence));
	sentence->block = block;
	sentence->type = BLOCK;
	return sentence;
}

Chord * ChordOfPatternsSemanticAction(PatternChord * pc) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Chord * chord = calloc(1, sizeof(Chord));
	chord->patternChord = pc;
	chord->type = PATTERN_CHORD;
	return chord;
}

Chord * ChordOfNotesSemanticAction(NoteChord * nc) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Chord * chord = calloc(1, sizeof(Chord));
	chord->noteChord = nc;
	chord->type = NOTE_CHORD;
	return chord;
}

PatternChord * PatternChordSemanticAction(CommaSeparatedPatterns * csp) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PatternChord * patternChord = calloc(1, sizeof(PatternChord));
	patternChord->patterns = csp;
	return patternChord;
}

NoteChord * NoteChordSemanticAction(CommaSeparatedNotes * csn) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NoteChord * noteChord = calloc(1, sizeof(NoteChord));
	noteChord->notes = csn;
	return noteChord;
}

CommaSeparatedPatterns * CSPSemanticAction(Pattern * pattern, CommaSeparatedPatterns * otherPatterns) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CommaSeparatedPatterns * csp = calloc(1, sizeof(CommaSeparatedPatterns));
	csp->pattern = pattern;
	csp->next = otherPatterns;
	return csp;
}

CommaSeparatedNotes * CSNSemanticAction(NoteAndOctave * note, CommaSeparatedNotes * otherNotes) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CommaSeparatedNotes * csn = calloc(1, sizeof(CommaSeparatedNotes));
	csn->note = note;
	csn->next = otherNotes;
	return csn;
}

Strum * StrumSemanticAction(CommaSeparatedNotes * csn, Duration * total, Duration * interval) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Strum * strum = calloc(1, sizeof(Strum));
	strum->notes = csn;
	strum->totalDuration = total;
	strum->interval = interval;
	return strum;
}

Arpeggio * ArpeggioSemanticAction(CommaSeparatedNotes * csn, Duration * total) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Arpeggio * arpeggio = calloc(1, sizeof(Arpeggio));
	arpeggio->notes = csn;
	arpeggio->totalDuration = total;
	return arpeggio;
}

Degree * DegreeSemanticAction(const int number, const int octave, Duration * duration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Degree * degree = calloc(1, sizeof(Degree));
	degree->number = number;
	degree->octave = octave;
	degree->duration = duration;
	return degree;
}

ID * IdentifierSemanticAction(const char *name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ID * id = calloc(1, sizeof(ID));
	int length = strlen(name);
	id->name = malloc((length+1)*sizeof(char));
	strcpy(id->name, name);
	return id;
}
