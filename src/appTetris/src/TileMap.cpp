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

void TileMap::Init(uint8_t Rows, uint8_t Cols, std::function<void(uint16_t, uint16_t)> SetWindowCallbackFuncPtrArg)
{
	Tilemap = std::vector<Tile>();
	sRows = Rows;
	sCols = Cols;

	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;

	for (size_t it = 0; it != static_cast<size_t>(Rows * Cols); ++it)
	{
		const size_t&& Row = (it / Cols);
		const size_t&& Col = (it % Cols);

		if (Row == Zero || Row == Rows - One || Col == Zero || Col == Cols - One)
		{
			Tilemap.emplace_back(Tile{ TileEnum::Border, it });
		}
		else
		{
			Tilemap.emplace_back(Tile{ TileEnum::Empty, it });
		}
	}

	SetWindowCallbackFuncPtrArg(Cols * Tile::Size, Rows * Tile::Size);
}

void TileMap::Update(TextureManager* const TextureManagerPtrArg, TetrominoeManager* const TetrominoeManagerPtrArg, SDLManager* const SDLManagerPtrArg,
	int8_t DirX, int8_t DirY)
{
	if (!TetrominoeManagerPtrArg || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	// update position of all tetrominoes
	TetrominoeManagerPtrArg->Update(this, DirX, DirY, sRows, sCols);

	// render all tiles
	for (auto& it : Tilemap)
	{
		it.Render(TextureManagerPtrArg, SDLManagerPtrArg, sRows, sCols);
	}
}

void TileMap::Clear()
{
	Tilemap.clear();
}