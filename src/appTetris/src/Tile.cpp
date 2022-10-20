#include "../include/Tile.h"

Tile::Tile(TileEnum TileEnum, uint8_t Index)
{
	IndexPosition = Index;
	Attribute = TileEnum;
}

void Tile::Render() const
{
	if (Attribute == TileEnum::Empty)
	{
		// load image from texture manager - fetch images using boost lib TODO integrate Boost lib
	}
	else
	{

	}
}