#include "../include/TetrominoeManager.h"

void TetrominoeManager::Add()
{
}

void TetrominoeManager::Remove()
{
}

void TetrominoeManager::Update(std::vector<Tile>& Tilemap, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	for (const auto& TetrominoeUniquePtr : TetrominoePool)
	{
		Tetrominoe* const TetrominoePtr = TetrominoeUniquePtr.get();
		if (!TetrominoePtr)
		{
			continue;
		}

		if (TetrominoePtr->IsLocked())
		{
			continue;
		}

		const bool&& IsMoveInBound = TetrominoePtr->IsMoveInBound(DirX, DirY, Rows, Cols);
		if (!IsMoveInBound)
		{
			continue;
		}

		const bool&& IsMoveOverlappingExistingTile = TetrominoePtr->IsMoveOverlappingExistingTile(Tilemap, DirX, DirY, Rows, Cols);
		if (IsMoveOverlappingExistingTile)
		{
			continue;
		}

		TetrominoePtr->Update(Tilemap, DirX, DirY, Rows, Cols);
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