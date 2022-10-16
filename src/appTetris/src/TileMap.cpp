#include "../include/TileMap.h"
#include "../include/Tetrominoe.h"

void TileMap::Init()
{
}

void TileMap::Update(int DirX, int DirY)
{
	for(auto& it : Tilemap)
	{
		Tetrominoe* Tetrominoe = it.get();
		if(!Tetrominoe)
		{
			continue;
		}

		static constexpr uint8_t TetrominoeSize = Tetrominoe->Size;
		static constexpr uint8_t Rows = Height / TetrominoeSize;
		static constexpr uint8_t Cols = Width / TetrominoeSize;

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