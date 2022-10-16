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
	Tilemap = std::vector<std::unique_ptr<Tetrominoe>>();
	this->Rows = Rows;
	this->Cols = Cols;

	for(auto it = 0; it != (Rows*Cols); ++it)
	{
		Tilemap.push_back(std::make_unique<Tetrominoe>());
	}

	for(auto& it : Tilemap)
	{
		/*Create Background Tiles*/
		/*Create Border Tiles*/
	}

	static constexpr uint8_t TetrominoeSize = Tetrominoe::Size;
	SetWindowCallback(Cols * TetrominoeSize, Rows * TetrominoeSize);
}

void TileMap::Update(int8_t DirX, int8_t DirY)
{
	for (auto& it : Tilemap)
	{
		Tetrominoe* Tetrominoe = it.get();
		if (!Tetrominoe)
		{
			continue;
		}

		/*Find Current Tetrominoe position*/
		/*Increase its position Index by using Args DirX & DirY*/
		/*Create WrapAround when Index > Rows*/
		/*Prevent WrapAround when Index > Cols && Index < 0*/
		/*Going Up is not Allowed*/
	}
}

void TileMap::Clear()
{
}