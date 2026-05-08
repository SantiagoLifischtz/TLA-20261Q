#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

void destroyNumber(Number * number) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (number != NULL) {
		free(number);
	}
}

void destroyExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				destroyExpression(expression->leftExpression);
				destroyExpression(expression->rightExpression);
				break;
			case FACTOR:
				destroyFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void destroyFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case NUMBER:
				destroyNumber(factor->number);
				break;
			case EXPRESSION:
				destroyExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void destroyProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		destroyDefinitions(program->definitions);
		destroyPlay(program->playBlock);
		free(program);
	}
}

void destroyDefinitions(Definitions * definitions) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (definitions != NULL) {
		destroyDefinition(definitions->def);
		destroyDefinitions(definitions->next);
		free(definitions);
	}
}

void destroyDefinition(Definition * definition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (definition != NULL) {
		switch (definition->type) {
			case CONFIG:
				destroyConfigSentence(definition->config);
				break;
			case PATTERN_DEF:
				destroyPatternDefinition(definition->pattern);
				break;
			case TRACK_DEF:
				destroyTrack(definition->track);
				break;
		}
		free(definition);
	}
}

void destroyPlay(Play * play) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (play != NULL) {
		if (play->type == TRACKS) {
			destroyTrackIDs(play->tracks);
		}
		free(play);
	}
}

void destroyID(ID * id) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (id != NULL) {
		free(id->name);
		free(id);
	}
}

void destroyTrackIDs(TrackIDs * trackIDs) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (trackIDs != NULL) {
		destroyID(trackIDs->id);
		destroyTrackIDs(trackIDs->next);
		free(trackIDs);
	}
}

void destroyConfigSentence(ConfigSentence * configSentence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (configSentence != NULL) {
		switch (configSentence->type) {
			case KEY:
				destroyKey(configSentence->key);
				break;
			case TEMPO:
				destroyTempo(configSentence->tempo);
				break;
			case PATTERN:
				break;
		}
		free(configSentence);
	}
}

void destroyTempo(Tempo * tempo) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (tempo != NULL) {
		destroyExpression(tempo->expression);
		free(tempo);
	}
}

void destroyKey(Key * key) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (key != NULL) {
		destroyNoteID(key->note);
		destroyMode(key->mode);
		free(key);
	}
}

void destroyMode(Mode * mode) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (mode != NULL) {
		free(mode);
	}
}

void destroyNoteID(NoteID * noteID) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (noteID != NULL) {
		free(noteID);
	}
}

void destroyTrack(Track * track) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (track != NULL) {
		destroyInstrument(track->instrument);
		destroySentences(track->sentences);
		free(track);
	}
}

void destroyInstrument(Instrument * instrument) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (instrument != NULL) {
		free(instrument->name);
		free(instrument);
	}
}

void destroySentences(Sentences * sentences) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (sentences != NULL) {
		destroySentence(sentences->sentence);
		destroySentences(sentences->next);
		free(sentences);
	}
}

void destroySentence(Sentence * sentence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (sentence != NULL) {
		switch (sentence->type) {
			case PATTERN:
				destroyPattern(sentence->patternSentence);
				break;
			case KEY:
				destroyKey(sentence->key);
				break;
			case TEMPO:
				destroyTempo(sentence->tempo);
				break;
		}
		free(sentence);
	}
}

void destroyPatternDefinition(PatternDefinition * patternDefinition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (patternDefinition != NULL) {
		destroyBlock(patternDefinition->block);
		free(patternDefinition);
	}
}

static void destroyPatternSentenceMembers(PatternSentence * patternSentence) {
	switch (patternSentence->type) {
		case INLINE_SENTENCE:
			destroyInlinePattern(patternSentence->inlinePattern);
			destroyWait(patternSentence->wait);
			break;
		case BLOCK_SENTENCE:
			destroyBlock(patternSentence->block);
			destroyWait(patternSentence->wait);
			break;
		case REPEAT:
			destroyRepeat(patternSentence->repeat);
			break;
		case STEP:
			destroyStep(patternSentence->step);
			break;
	}
}

void destroyPatternSentence(PatternSentence * patternSentence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (patternSentence != NULL) {
		destroyPatternSentenceMembers(patternSentence);
		free(patternSentence);
	}
}

void destroyPattern(Pattern * pattern) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (pattern != NULL) {
		switch (pattern->type) {
			case INLINE:
				destroyInlinePattern(pattern->inlinePattern);
				break;
			case BLOCK:
				destroyBlock(pattern->block);
				break;
		}
		free(pattern);
	}
}

void destroyBlock(Block * block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (block != NULL) {
		destroySentences(block->sentences);
		free(block);
	}
}

void destroyWait(Wait * wait) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (wait != NULL) {
		destroyDuration(wait->duration);
		free(wait);
	}
}

void destroyInlinePattern(InlinePattern * inlinePattern) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (inlinePattern != NULL) {
		switch (inlinePattern->type) {
			case NOTE:
				destroyNote(inlinePattern->note);
				break;
			case REST:
				destroyRest(inlinePattern->rest);
				break;
			case CHORD:
				destroyChord(inlinePattern->chord);
				break;
			case STRUM:
				destroyStrum(inlinePattern->strum);
				break;
			case ARPEGGIO:
				destroyArpeggio(inlinePattern->arpeggio);
				break;
			case DEGREE:
				destroyDegree(inlinePattern->degree);
				break;
			case PATTERN_ID:
				destroyID(inlinePattern->id);
				break;
		}
		free(inlinePattern);
	}
}

void destroyNote(Note * note) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (note != NULL) {
		destroyNoteAndOctave(note->noteAndOctave);
		destroyDuration(note->duration);
		free(note);
	}
}

void destroyNoteAndOctave(NoteAndOctave * noteAndOctave) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (noteAndOctave != NULL) {
		destroyNoteID(noteAndOctave->note);
		free(noteAndOctave);
	}
}

void destroyDuration(Duration * duration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (duration != NULL) {
		destroyExpression(duration->expression);
		free(duration);
	}
}

void destroyRest(Rest * rest) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (rest != NULL) {
		destroyDuration(rest->duration);
		free(rest);
	}
}

void destroyRepeat(Repeat * repeat) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (repeat != NULL) {
		destroyPatternSentenceMembers(repeat->sentence);
		free(repeat);
	}
}

void destroyStep(Step * step) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (step != NULL) {
		destroyDuration(step->interval);
		destroyStepBlock(step->block);
		free(step);
	}
}

void destroyStepBlock(StepBlock * stepBlock) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (stepBlock != NULL) {
		destroyNoWaitSentences(stepBlock->sentences);
		free(stepBlock);
	}
}

void destroyNoWaitSentences(NoWaitSentences * noWaitSentences) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (noWaitSentences != NULL) {
		destroyNoWaitSentence(noWaitSentences->sentence);
		destroyNoWaitSentences(noWaitSentences->next);
		free(noWaitSentences);
	}
}

void destroyNoWaitSentence(NoWaitSentence * noWaitSentence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (noWaitSentence != NULL) {
		destroyPattern(noWaitSentence->pattern);
		free(noWaitSentence);
	}
}

void destroyChord(Chord * chord) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (chord != NULL) {
		switch (chord->type) {
			case PATTERN_CHORD:
				destroyPatternChord(chord->patternChord);
				break;
			case NOTE_CHORD:
				destroyNoteChord(chord->noteChord);
				break;
		}
		free(chord);
	}
}

void destroyPatternChord(PatternChord * patternChord) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (patternChord != NULL) {
		destroyCommaSeparatedPatterns(patternChord->patterns);
		free(patternChord);
	}
}

void destroyNoteChord(NoteChord * noteChord) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (noteChord != NULL) {
		destroyCommaSeparatedNotes(noteChord->notes);
		free(noteChord);
	}
}

void destroyCommaSeparatedPatterns(CommaSeparatedPatterns * commaSeparatedPatterns) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (commaSeparatedPatterns != NULL) {
		destroyPattern(commaSeparatedPatterns->pattern);
		destroyCommaSeparatedPatterns(commaSeparatedPatterns->next);
		free(commaSeparatedPatterns);
	}
}

void destroyCommaSeparatedNotes(CommaSeparatedNotes * commaSeparatedNotes) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (commaSeparatedNotes != NULL) {
		destroyNoteAndOctave(commaSeparatedNotes->note);
		destroyCommaSeparatedNotes(commaSeparatedNotes->next);
		free(commaSeparatedNotes);
	}
}

void destroyStrum(Strum * strum) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (strum != NULL) {
		destroyCommaSeparatedNotes(strum->notes);
		destroyDuration(strum->totalDuration);
		destroyDuration(strum->interval);
		free(strum);
	}
}

void destroyArpeggio(Arpeggio * arpeggio) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arpeggio != NULL) {
		destroyCommaSeparatedNotes(arpeggio->notes);
		destroyDuration(arpeggio->totalDuration);
		free(arpeggio);
	}
}

void destroyDegree(Degree * degree) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (degree != NULL) {
		destroyDuration(degree->duration);
		free(degree);
	}
}
