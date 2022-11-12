#include "../include/TetrominoeManager.h"
#include "../include/Tetrominoe.h"
#include "../include/TileMap.h"

void TetrominoeManager::Update(TileMap* const TilemapPtr, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	if(!TilemapPtr)
	{
		return;
	}

	for (auto& it : TetrominoePool)
	{
		Tetrominoe* const Tetrominoe = it.get();
		if (!Tetrominoe)
		{
			continue;
		}

		const bool&& IsMoveInBound = Tetrominoe->IsMoveInBound(DirX, DirY, Rows, Cols);
		if (!IsMoveInBound)
		{
			continue;
		}

		const bool&& IsMoveOverlappingExistingTile = Tetrominoe->IsMoveOverlappingExistingTile(DirX, DirY, Rows, Cols);
		if (!IsMoveOverlappingExistingTile)
		{
			continue;
		}

		Tetrominoe->Update(DirX, DirY, Rows, Cols);
	}
}

void TetrominoeManager::Flip() const
{
	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->Flip();
}