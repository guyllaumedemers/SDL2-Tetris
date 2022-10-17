#include "../include/TileMap.h"
#include "../include/Tetrominoe.h"

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

	static constexpr uint8_t TetrominoeSize = Tile::Size;
	SetWindowCallback(Cols * TetrominoeSize, Rows * TetrominoeSize);
}

void TileMap::Update(int8_t DirX, int8_t DirY)
{
	/*Find Current Tetrominoe position*/
	/*Increase its position Index by using Args DirX & DirY*/
	/*Create WrapAround when Index > Rows*/
	/*Prevent WrapAround when Index > Cols && Index < 0*/
	/*Going Up is not Allowed*/
}

void TileMap::Clear()
{
}