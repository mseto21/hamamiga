#pragma once
#include "Types.h"
#include "TileMap.h"

// Forward Declarations
struct TileMap;

struct Zone {
	String128 name;		/* The name of the current zone. */
	String128 tileset;	/* Path to the zone's tileset, which will be loaded by tcache */
	TileMap tileMap;
};

void Zone_Create(const char* name, Zone* embeddedZones);