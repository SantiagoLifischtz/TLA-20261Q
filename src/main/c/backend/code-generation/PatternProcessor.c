#include "PatternProcessor.h"
#include "../../support/logging/Logger.h"

static Logger * _logger = NULL;
static MidiEventListADT _conductorTrack = NULL;

void setPatternProcessorLogger(Logger * logger) {
	_logger = logger;
}

void setPatternProcessorConductorTrack(MidiEventListADT track) {
	_conductorTrack = track;
}

/* PRIVATE FUNCTIONS */

static uint32_t _dispatchInlinePattern(MidiEventListADT events, InlinePattern * iPtrn, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context);
static bool _appendNoteEvent(MidiEventListADT events, NoteAndOctave * noteAndOctave, uint32_t playhead, Scale * scale, unsigned char channel, uint32_t durationTicks);
static bool _appendTempoAtPlayhead(Tempo * tempo, uint32_t playhead, bool strictValidation);
static uint32_t _processPatternSentenceAtPlayhead(
	MidiEventListADT events,
	PatternSentence * patternSentence,
	uint32_t playhead,
	Scale * scale,
	unsigned char channel,
	DefinitionContext * context,
	bool strictValidation,
	bool * succeeded
);

static uint32_t _dispatchInlinePattern(MidiEventListADT events, InlinePattern * iPtrn, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (iPtrn == NULL) {
        return 0;
    }

	switch (iPtrn->type) {
		case NOTE:
			return processNote(events, iPtrn->note, playhead, scale, channel);
		case REST:
			return processRest(iPtrn->rest);
		case CHORD:
			if (iPtrn->chord->type == NOTE_CHORD) {
                return processNoteChord(events, iPtrn->chord->noteChord, playhead, scale, channel);
            }
			else {
                return processPatternChord(events, iPtrn->chord->patternChord, playhead, scale, channel, context);
            }
		case STRUM:
			return processStrum(events, iPtrn->strum, playhead, scale, channel);
		case ARPEGGIO:
			return processArpeggio(events, iPtrn->arpeggio, playhead, scale, channel);
		case PATTERN_ID:
			return processPatternId(events, iPtrn->id, playhead, scale, channel, context);
		default:
			return 0;
	}
}

static bool _appendNoteEvent(MidiEventListADT events, NoteAndOctave * noteAndOctave, uint32_t playhead, Scale * scale, unsigned char channel, uint32_t durationTicks) {
	if (noteAndOctave == NULL) {
        return false;
    }

	MidiNoteResult noteResult;
	switch (noteAndOctave->type) {
		case ABSOLUTE:
			noteResult = getMidiAbsoluteNote(noteAndOctave->note->value, noteAndOctave->octave);
			break;
		case FROM_DEGREE:
			noteResult = getMidiDegreeNote(noteAndOctave->degree->number, noteAndOctave->degree->octave, scale);
			break;
		case FROM_STRING: {
			MidiPercussionResult percussionResult = getMidiPercussionNumber(noteAndOctave->identifier);
			noteResult.succeeded = percussionResult.succeeded;
			noteResult.value = percussionResult.value;
			break;
		}
		default:
			return false;
	}

	if (!noteResult.succeeded) {
		if (noteAndOctave->type == FROM_STRING) {
			logError(_logger, "Percussion unkown: %s", noteAndOctave->identifier);
		} else if (noteAndOctave->type == ABSOLUTE) {
			logError(_logger, "Invalid note: %c%c", noteAndOctave->note->value, noteAndOctave->octave);
		} else if (noteAndOctave->type == FROM_DEGREE) {
			logError(_logger, "Invalid degree: %d in octave %d",
			         noteAndOctave->degree->number, noteAndOctave->degree->octave);
		}

		return false;
	}

	unsigned char midiChannel = channel;
	if (noteAndOctave->type == FROM_STRING) {
		midiChannel = MIDI_PERCUSSION_CHANNEL;
	}

	uint8_t noteOn[3] = { MIDI_NOTE_ON | midiChannel, noteResult.value, MIDI_DEFAULT_VELOCITY };
	if (!midiEventListAppend(events, playhead, noteOn, 3)) {
        return false;
    }

	uint8_t noteOff[3] = { MIDI_NOTE_OFF | midiChannel, noteResult.value, 0 };
	if (!midiEventListAppend(events, playhead + durationTicks, noteOff, 3)) {
        return false;
    }

	return true;
}

static bool _appendTempoAtPlayhead(Tempo * tempo, uint32_t playhead, bool strictValidation) {
	if (tempo == NULL) {
		return !strictValidation;
	}

	FloatEvaluation eval = evaluateExpression(tempo->expression);
	if (eval.succeeded && eval.value > 0.0f) {
		if (_conductorTrack == NULL) {
			return !strictValidation;
		}

		uint8_t td[6];
		buildTempoEvent(td, MIDI_TEMPO_FROM_BPM(eval.value));

		if (!midiEventListAppend(_conductorTrack, playhead, td, 6)) {
			logError(_logger, "Set Tempo event append fail.");
			return false;
		}

		return true;
	}

	if (strictValidation) {
		logError(_logger, "Invalid tempo expression.");
		return false;
	}

	return true;
}

static uint32_t _processPatternSentenceAtPlayhead(
	MidiEventListADT events,
	PatternSentence * patternSentence,
	uint32_t playhead,
	Scale * scale,
	unsigned char channel,
	DefinitionContext * context,
	bool strictValidation,
	bool * succeeded
) {
	uint32_t advance = dispatchPatternSentence(events, patternSentence, playhead, scale, channel, context);

	if (strictValidation && advance == 0) {
		logError(_logger, "Pattern sentence processing failed.");
		*succeeded = false;
		return 0;
	}

	return advance;
}

SentencesResult processSentences(
	MidiEventListADT events,
	Sentences * sentences,
	uint32_t playhead,
	Scale * scale,
	unsigned char channel,
	DefinitionContext * context,
	bool strictValidation
) {
	SentencesResult result = { .succeeded = true, .ticksAdvanced = 0 };

	if (sentences == NULL) {
		return result;
	}

	uint32_t localPlayhead = playhead;
	for (Sentences * s = sentences; s != NULL; s = s->next) {
		if (s->sentence == NULL) {
			continue;
		}

		switch (s->sentence->type) {
			case KEY:
				applyKeyToScale(s->sentence->key, scale);
				break;
			case TEMPO:
				if (!_appendTempoAtPlayhead(s->sentence->tempo, localPlayhead, strictValidation)) {
					result.succeeded = false;
					return result;
				}
				break;
			case PATTERN: {
				uint32_t advance = _processPatternSentenceAtPlayhead(
					events,
					s->sentence->patternSentence,
					localPlayhead,
					scale,
					channel,
					context,
					strictValidation,
					&result.succeeded
				);

				if (!result.succeeded) {
					return result;
				}

				localPlayhead += advance;
				break;
			}
		}
	}

	result.ticksAdvanced = localPlayhead - playhead;
	return result;
}

/* PUBLIC FUNCTIONS */

uint32_t processNote(MidiEventListADT events, Note * note, uint32_t playhead, Scale * scale, unsigned char channel) {
	if (note == NULL || note->noteAndOctave == NULL || note->duration == NULL) {
		return 0;
	}

	uint32_t durationTicks = evaluateDurationToTicks(note->duration->expression);
	if (durationTicks == 0) {
		return 0;
	}

	if (!_appendNoteEvent(events, note->noteAndOctave, playhead, scale, channel, durationTicks)) {
		return 0;
	}

	return durationTicks;
}

uint32_t processRest(Rest * rest) {
	if (rest == NULL || rest->duration == NULL) {
		return 0;
	}

	return evaluateDurationToTicks(rest->duration->expression);
}

uint32_t processNoteChord(MidiEventListADT events, NoteChord * chord, uint32_t playhead, Scale * scale,unsigned char channel) {
	if (chord == NULL || chord->notes == NULL || chord->duration == NULL) {
		return 0;
	}

	uint32_t durationTicks = evaluateDurationToTicks(chord->duration->expression);
	if (durationTicks == 0) {
		return 0;
	}

	for (CommaSeparatedNotes * n = chord->notes; n != NULL; n = n->next) {
		if (n->note == NULL) {
            continue;
        }
		if (!_appendNoteEvent(events, n->note, playhead, scale, channel, durationTicks)) {
			return 0;
		}
	}

	return durationTicks;
}

uint32_t processStrum(MidiEventListADT events, Strum * strum, uint32_t playhead, Scale * scale, unsigned char channel) {
	if (strum == NULL || strum->notes == NULL || strum->totalDuration == NULL || strum->interval == NULL) {
		return 0;
	}

	uint32_t intervalTicks = evaluateDurationToTicks(strum->interval->expression);
	if (intervalTicks == 0) {
		return 0;
	}

	uint32_t totalTicks = evaluateDurationToTicks(strum->totalDuration->expression);
	if (totalTicks == 0) {
		return 0;
	}

	uint32_t noteCount = 0;
	for (CommaSeparatedNotes * n = strum->notes; n != NULL; n = n->next) {
		noteCount++;
	}
	if (noteCount == 0 || noteCount * intervalTicks > totalTicks) {
		return 0;
	}

	uint32_t localPlayhead = playhead;
	for (CommaSeparatedNotes * n = strum->notes; n != NULL; n = n->next) {
		if (n->note == NULL) { localPlayhead += intervalTicks; continue; }

		uint32_t noteDuration = (playhead + totalTicks) - localPlayhead;
		if (!_appendNoteEvent(events, n->note, localPlayhead, scale, channel, noteDuration)) {
			return 0;
		}

		localPlayhead += intervalTicks;
	}

	return totalTicks;
}

uint32_t processArpeggio(MidiEventListADT events, Arpeggio * arp, uint32_t playhead, Scale * scale, unsigned char channel) {
	if (arp == NULL || arp->notes == NULL || arp->totalDuration == NULL) {
		return 0;
	}

	uint32_t totalTicks = evaluateDurationToTicks(arp->totalDuration->expression);
	if (totalTicks == 0) return 0;

	uint32_t noteCount = 0;
	for (CommaSeparatedNotes * n = arp->notes; n != NULL; n = n->next) {
		noteCount++;
	}
	if (noteCount == 0) return 0;

	uint32_t noteDuration = totalTicks / noteCount;
	if (noteDuration == 0) return 0;

	uint32_t localPlayhead = playhead;
	for (CommaSeparatedNotes * n = arp->notes; n != NULL; n = n->next) {
		if (n->note == NULL) { localPlayhead += noteDuration; continue; }

		if (!_appendNoteEvent(events, n->note, localPlayhead, scale, channel, noteDuration)) {
			return 0;
		}

		localPlayhead += noteDuration;
	}

	return totalTicks;
}

uint32_t processBlock(MidiEventListADT events, Block * block, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (block == NULL || block->sentences == NULL) {
		return 0;
	}

	SentencesResult result = processSentences(events, block->sentences, playhead, scale, channel, context, false);
	return result.succeeded ? result.ticksAdvanced : 0;
}

uint32_t processPatternId(MidiEventListADT events, Identifier * id, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (id == NULL || id->name == NULL || context == NULL) {
		return 0;
	}

	PatternEntry * entry = findPatternEntry(context, id->name);
	if (entry == NULL || entry->definition == NULL || entry->definition->block == NULL) {
		logError(_logger, "Unknown pattern: %s", id->name);

		return 0;
	}

	return processBlock(events, entry->definition->block, playhead, scale, channel, context);
}

uint32_t processRepeat(MidiEventListADT events, Repeat * repeat, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (repeat == NULL || repeat->count <= 0 || repeat->sentence == NULL) {
		return 0;
	}

	uint32_t localPlayhead = playhead;
	for (int i = 0; i < repeat->count; i++) {
		localPlayhead += dispatchPatternSentence(events, repeat->sentence, localPlayhead, scale, channel, context);
	}

	return localPlayhead - playhead;
}

uint32_t processStep(MidiEventListADT events, Step * step, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (step == NULL || step->interval == NULL || step->block == NULL || step->block->sentences == NULL) {
		return 0;
	}

	uint32_t intervalTicks = evaluateDurationToTicks(step->interval->expression);
	if (intervalTicks == 0) {
        return 0;
    }

	uint32_t localPlayhead = playhead;
	for (NoWaitSentences * nws = step->block->sentences; nws != NULL; nws = nws->next) {
		if (nws->sentence == NULL) {
            localPlayhead += intervalTicks;
            continue;
        }

		switch (nws->sentence->type) {
			case INLINE:
				_dispatchInlinePattern(events, nws->sentence->inlinePattern, localPlayhead, scale, channel, context);
				break;
			case BLOCK:
				processBlock(events, nws->sentence->block, localPlayhead, scale, channel, context);
				break;
		}
		localPlayhead += intervalTicks;
	}

	return localPlayhead - playhead;
}

uint32_t processPatternChord(MidiEventListADT events, PatternChord * chord, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (chord == NULL || chord->patterns == NULL) {
		return 0;
	}

	uint32_t longest = 0;
	for (CommaSeparatedPatterns * p = chord->patterns; p != NULL; p = p->next) {
		if (p->pattern == NULL) continue;

		uint32_t duration;
		switch (p->pattern->type) {
			case INLINE:
				duration = _dispatchInlinePattern(events, p->pattern->inlinePattern, playhead, scale, channel, context);
				break;
			case BLOCK:
				duration = processBlock(events, p->pattern->block, playhead, scale, channel, context);
				break;
			default:
				return 0;
		}
		if (duration > longest) longest = duration;
	}

	return longest;
}

uint32_t dispatchPatternSentence(MidiEventListADT events, PatternSentence * pSntc, uint32_t playhead, Scale * scale, unsigned char channel, DefinitionContext * context) {
	if (pSntc == NULL) {
        return 0;
    }

	switch (pSntc->type) {
		case INLINE_SENTENCE: {
			uint32_t duration = _dispatchInlinePattern(events, pSntc->inlinePattern, playhead, scale, channel, context);
			
            if (pSntc->wait != NULL) {
				return evaluateDurationToTicks(pSntc->wait->duration->expression);
			}

			return duration;
		}
		case BLOCK_SENTENCE: {
			uint32_t duration = processBlock(events, pSntc->block, playhead, scale, channel, context);
			
            if (pSntc->wait != NULL) {
				return evaluateDurationToTicks(pSntc->wait->duration->expression);
			}
			
            return duration;
		}
		case REPEAT:
			return processRepeat(events, pSntc->repeat, playhead, scale, channel, context);
		case STEP:
			return processStep(events, pSntc->step, playhead, scale, channel, context);
		default:
			return 0;
	}
}

