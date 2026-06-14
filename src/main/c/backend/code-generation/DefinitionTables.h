#ifndef DEFINITION_TABLES_HEADER
#define DEFINITION_TABLES_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "util/ScaleUtils.h"
#include <stdbool.h>

//Busqueda de pattern por ID 
typedef struct PatternEntry {
	Identifier * id;
	PatternDefinition * definition;
	struct PatternEntry * next;
} PatternEntry;

//Busqueda de track por ID
typedef struct TrackEntry {
	Identifier * id;
	Track * track;
	bool selectedForExport; //el track se exporta al midi
	struct TrackEntry * next;
} TrackEntry;

// Mapeo del instrumento a canal MIDI
typedef struct InstrumentChannelEntry {
	char * instrumentName;
	unsigned char channel;
	struct InstrumentChannelEntry * next;
} InstrumentChannelEntry;

typedef struct DefinitionContext {
	PatternEntry * patterns;
	TrackEntry * tracks;
	InstrumentChannelEntry * instrumentChannels;
	float globalTempoBpm; 
	bool globalTempoSet;
	Scale globalScale; 
	bool globalScaleSet;
} DefinitionContext;

typedef struct {
	bool succeeded;
} DefinitionTablesResult;

DefinitionTablesResult buildDefinitionContext(const Program * program, DefinitionContext * context);
void destroyDefinitionContext(DefinitionContext * context);
TrackEntry * findTrackEntry(const DefinitionContext * context, const char * name);
PatternEntry * findPatternEntry(const DefinitionContext * context, const char * name);
bool assignInstrumentChannel(DefinitionContext * context, const char * instrumentName);
unsigned char getInstrumentChannel(const DefinitionContext * context, const char * instrumentName);

#endif
