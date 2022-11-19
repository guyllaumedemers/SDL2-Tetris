#include "../include/TileMap.h"

/// <summary>
/// Static Fields
/// </summary>
/// <param name="Rows"></param>
/// <param name="Cols"></param>
uint8_t TileMap::sRows = 0;
uint8_t TileMap::sCols = 0;
//@end()

void TileMap::Initialize(uint8_t Rows, uint8_t Cols, const std::function<void(uint16_t, uint16_t)>& SetWindowFncPtrArg)
{
	Tiles = std::vector<Tile>();
	sRows = Rows;
	sCols = Cols;

	const uint8_t&& Zero = 0;
	const uint8_t&& One = 1;

	for (size_t Index = 0; Index != static_cast<size_t>(Rows * Cols); ++Index)
	{
		const size_t&& Row = (Index / Cols);
		const size_t&& Col = (Index % Cols);

		if (Row == Zero || Row == Rows - One || Col == Zero || Col == Cols - One)
		{
			Tiles.emplace_back(Tile{ TileEnum::Border, std::string("Undefined"), Index });
		}
		else
		{
			Tiles.emplace_back(Tile{ TileEnum::Empty, std::string("Undefined"), Index });
		}
	}

	SetWindowFncPtrArg(Cols * Tile::Size, Rows * Tile::Size);
}

void TileMap::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg, int8_t DirX, int8_t DirY)
{
	if (!TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	for (const auto& it : Tiles)
	{
		it.Render(TextureManagerPtrArg, SDLManagerPtrArg, sRows, sCols);
	}
}

void TileMap::Clear()
{
	Tiles.clear();
}

void TileMap::CheckRowCompletion(uint16_t IndexPosition)
{
}
