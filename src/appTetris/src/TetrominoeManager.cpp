#include "../include/TetrominoeManager.h"
#include "../include/Tetrominoe.h"

void TetrominoeManager::Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	for (auto& it : TetrominoePool)
	{
		Tetrominoe* const Tetrominoe = it.get();
		if (!Tetrominoe || Tetrominoe->IsLocked())
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