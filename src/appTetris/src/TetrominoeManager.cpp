#include "../include/TetrominoeManager.h"
#include "../include/Tetrominoe.h"
#include "../include/TileMap.h"

void TetrominoeManager::Update(TileMap* const TilemapPtrArg, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	if (!TilemapPtrArg)
	{
		return;
	}

	for (auto& TetrominoeUniquePtr : TetrominoePool)
	{
		Tetrominoe* const TetrominoePtr = TetrominoeUniquePtr.get();
		if (!TetrominoePtr)
		{
			continue;
		}

		const bool&& IsMoveInBound = TetrominoePtr->IsMoveInBound(DirX, DirY, Rows, Cols);
		if (!IsMoveInBound)
		{
			continue;
		}

		const bool&& IsMoveOverlappingExistingTile = TetrominoePtr->IsMoveOverlappingExistingTile(TilemapPtrArg->Tilemap, DirX, DirY, Rows, Cols);
		if (!IsMoveOverlappingExistingTile)
		{
			continue;
		}

		TetrominoePtr->Update(TilemapPtrArg->Tilemap, DirX, DirY, Rows, Cols);
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