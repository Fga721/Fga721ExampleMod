#pragma once
#include "MemUtils.h"
#include "SexyTypes.h"

#define GRID_TILE_WIDTH 64.0f
#define GRID_TILE_HEIGHT 76.0f
#define GRID_TILE_TOP 160.0f
#define GRID_TILE_LEFT 200.0f

#define BOARD_START_POS 232.0f
#define BOARD_END_POS 776.0f

class PresentRecord
{
public:
	SexyString PresentTableName;
	uint Seed;
	uint TimeAwarded;
	uint TimeOpened;
};

static_assert(sizeof(PresentRecord) == 40);
static_assert(offsetof(PresentRecord, Seed) == 24);
static_assert(offsetof(PresentRecord, TimeOpened) == 32);

class RowPickingItem
{
public:
	int m_row;
	float m_weight;
	float m_lastPicked;
	float m_secondLastPicked;
};

static_assert(sizeof(RowPickingItem) == 16);
static_assert(offsetof(RowPickingItem, m_row) == 0);

class LootStats
{
public:
	int LootTotalCount;
	int SilverCount;
	int GoldCount;
	int DiamondCount;
	int PowerupCount;
	int NoKeyCount;
	int KeyCount;
};

static_assert(sizeof(LootStats) == 28);
static_assert(offsetof(LootStats, LootTotalCount) == 0);
static_assert(offsetof(LootStats, NoKeyCount) == 20);

enum ThymedEventTrack
{
	track1,
	track2,
	track3
};

class Zombie;
class GridItem;
class LevelDefinition;
class LevelModuleManager;
class Collectable;

inline uint64_t GetSexyApp() {
	return *(uint64_t*)Fga721GetActualOffset(0x25A4618);
}

inline uint64_t GetLawnApp() {
	return *(uint64_t*)Fga721GetActualOffset(0x1E77D8C);
}