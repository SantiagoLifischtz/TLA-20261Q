#ifndef PLAY_BLOCK_HEADER
#define PLAY_BLOCK_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "DefinitionTables.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool succeeded;
	uint16_t exportTrackCount;
} PlayBlockResult;


PlayBlockResult resolvePlayBlock(const Program * program, DefinitionContext * context);

#endif
