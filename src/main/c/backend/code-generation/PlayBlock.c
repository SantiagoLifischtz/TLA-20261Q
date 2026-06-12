#include "PlayBlock.h"

#include "../../support/logging/Logger.h"
#include <string.h>

static Logger * _logger = NULL;

static PlayBlockResult _failedResult(void) {
	PlayBlockResult result = { .succeeded = false, .exportTrackCount = 0 };
	return result;
}

static PlayBlockResult _succeededResult(uint16_t exportTrackCount) {
	PlayBlockResult result = { .succeeded = true, .exportTrackCount = exportTrackCount };
	return result;
}

static bool _isDuplicatePlayId(const TrackIDs * playTracks, const char * name) {
	for (const TrackIDs * current = playTracks->next; current != NULL; current = current->next) {
		if (strcmp(current->id->name, name) == 0) {
			return true;
		}
	}
	return false;
}

// canales solo para los tracks exportados, los demás no consumen el canal
static bool _assignChannelsForExportTracks(DefinitionContext * context) {
	for (TrackEntry * entry = context->tracks; entry != NULL; entry = entry->next) {
		if (!entry->selectedForExport) {
			continue;
		}
		const char * instrumentName = entry->track != NULL && entry->track->instrument != NULL ? entry->track->instrument->name: NULL;
		if (!assignInstrumentChannel(context, instrumentName)) {
			return false;
		}
	}
	return true;
}

// Marca los tracks que van al .mid según el play block
// Al midi solo escribimos los eventos de los tracks marcados
PlayBlockResult resolvePlayBlock(const Program * program, DefinitionContext * context) {
	if (_logger == NULL) {
		_logger = createLogger("PlayBlock");
	}
	if (program == NULL || context == NULL) {
		logError(_logger, "Cannot resolve play block from a null program or context.");
		return _failedResult();
	}
	if (program->playBlock == NULL) {
		logError(_logger, "Program is missing a play block.");
		return _failedResult();
	}

	Play * playBlock = program->playBlock;
	uint16_t exportTrackCount = 0;

	if (playBlock->type == PLAY_ALL) {
		for (TrackEntry * entry = context->tracks; entry != NULL; entry = entry->next) {
			entry->selectedForExport = true;
			exportTrackCount++;
		}
	}
	else if (playBlock->type == TRACKS) {
		// play { id1, id2, etc }; 
		// si el ID no existe o se repite falla
		for (TrackIDs * playTrack = playBlock->tracks; playTrack != NULL; playTrack = playTrack->next) {
			if (playTrack->id == NULL || playTrack->id->name == NULL) {
				logError(_logger, "Invalid track identifier in play block.");
				return _failedResult();
			}
			if (_isDuplicatePlayId(playTrack, playTrack->id->name)) {
				logError(_logger, "Duplicate track identifier \"%s\" in play block.", playTrack->id->name);
				return _failedResult();
			}
			TrackEntry * entry = findTrackEntry(context, playTrack->id->name);
			if (entry == NULL) {
				logError(_logger, "Play block references unknown track \"%s\".", playTrack->id->name);
				return _failedResult();
			}
			if (!entry->selectedForExport) {
				entry->selectedForExport = true;
				exportTrackCount++;
			}
		}
	}
	else {
		logError(_logger, "Unknown play block type: %d", playBlock->type);
		return _failedResult();
	}

	if (!_assignChannelsForExportTracks(context)) {
		return _failedResult();
	}

	logDebugging(_logger, "Play block resolved: %u track(s) selected for export.", exportTrackCount);
	return _succeededResult(exportTrackCount);
}
