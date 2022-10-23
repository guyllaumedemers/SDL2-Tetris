#include "../include/TileMap.h"
#include "../include/TetrominoeManager.h"

/// <summary>
/// Static Fields
/// </summary>
/// <param name="Rows"></param>
/// <param name="Cols"></param>
uint8_t TileMap::sRows = 0;
uint8_t TileMap::sCols = 0;
//@end()

void TileMap::Init(uint8_t Rows, uint8_t Cols, std::function<void(uint16_t, uint16_t)> SetWindowCallback)
{
	Tilemap = std::vector<Tile>();
	sRows = Rows;
	sCols = Cols;

	for (size_t it = 0; it != static_cast<size_t>(Rows * Cols); ++it)
	{
		const size_t Row = (it / Cols);
		const size_t Col = (it % Cols);

		if (Row == 0 || Row == Rows - 1 || Col == 0 || Col == Cols - 1)
		{
			Tilemap.emplace_back(Tile(TileEnum::Border, it));
		}
		else
		{
			Tilemap.emplace_back(Tile(TileEnum::Empty, it));
		}
	}

	SetWindowCallback(Cols * Tile::Size, Rows * Tile::Size);
}

void TileMap::Update(TextureManager* const TextureManagerPtr, TetrominoeManager* const TetrominoeManagerPtr, SDLManager* const SDLManagerPtr, int8_t DirX, int8_t DirY)
{
	if (!TetrominoeManagerPtr || !TextureManagerPtr || !SDLManagerPtr)
	{
		return;
	}

	// update position of all tetrominoes
	TetrominoeManagerPtr->Update(DirX, DirY, sRows, sCols);
	// render all tiles
	for (auto& it : Tilemap)
	{
		it.Render(TextureManagerPtr, SDLManagerPtr, sRows, sCols);
	}
}

void TileMap::Clear()
{
	Tilemap.clear();
}