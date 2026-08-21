#include "SexyTypes.h"
#include "MemUtils.h"

#define GRID_TILE_WIDTH 64.0f
#define GRID_TILE_HEIGHT 76.0f
#define GRID_TILE_TOP 160.0
#define GRID_TILE_LEFT 200.0

#define BOARD_START_POS 232.0f
#define BOARD_END_POS 776.0f

class Board
{
public:
	char pad_0000[148];
	int m_columnCount;
	int m_rowCount;
	char pad_009C[676];
	Sexy::Rect m_lawnRect;

	// Get the Board instance from gLawnApp
	static Board* GetBoard()
	{
		return CallFunc<Board*>(0xCE4D8C);
	}

	void ChangeState(int newState)
	{
		CallFunc<void, Board*, int>(0x71B1AC, this, newState);
	}
};

inline uint64_t GetSexyApp() {
	return *(uint64_t*)Fga721GetActualOffset(0x25A4618);
}

inline uint64_t GetLawnApp() {
	return *(uint64_t*)Fga721GetActualOffset(0x1E77D8C);
}
