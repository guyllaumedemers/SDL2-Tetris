#include "../include/TileMap.h"
#include "../include/TetrominoeManager.h"

/// <summary>
/// Static Fields
/// </summary>
/// <param name="Rows"></param>
/// <param name="Cols"></param>
uint8_t TileMap::Rows = 0;
uint8_t TileMap::Cols = 0;
//@end()

void TileMap::Init(uint8_t Rows, uint8_t Cols, std::function<void(uint16_t, uint16_t)> SetWindowCallback)
{
	Tilemap = std::vector<Tile>();
	this->Rows = Rows;
	this->Cols = Cols;

	for (auto it = 0; it != (Rows * Cols); ++it)
	{
		const uint8_t Row = (it / Rows);
		const uint8_t Col = (it % Cols);

		if (Row == 0 || Row == Rows - 1 || Col == 0 || Col == Cols - 1)
		{
			Tilemap.push_back(Tile(TileEnum::Border, it));
		}
		else
		{
			Tilemap.push_back(Tile(TileEnum::Empty, it));
		}
	}

	static constexpr uint8_t TileSize = Tile::Size;
	SetWindowCallback(Cols * TileSize, Rows * TileSize);
}

void TileMap::Update(TetrominoeManager* const TetrominoeManagerPtr, int8_t DirX, int8_t DirY)
{
	if (!TetrominoeManagerPtr)
	{
		return;
	}
	// update position of all tetrominoes
	TetrominoeManagerPtr->Update(DirX, DirY, Rows, Cols);
	// render all tiles
	for (const auto& it : Tilemap)
	{
		it.Render();
	}
}

void TileMap::Clear()
{
	Tilemap.clear();
}