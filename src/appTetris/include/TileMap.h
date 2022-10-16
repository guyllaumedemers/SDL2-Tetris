#ifndef INCLUDED_TILEMAP
#define INCLUDED_TILEMAP

#ifndef INCLUDED_COLLECTION_ARRAY
#define INCLUDED_COLLECTION_ARRAY
#include <array>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

class TileMap final
{
	static uint16_t Width = 200;
	static uint16_t Height = 300;
	std::array<std::unique_ptr<class Tetrominoe>, NULL> Tilemap = std::array<std::unique_ptr<class Tetrominoe>, NULL>();

	friend class Tetrominoe;

public:
	TileMap() = default;
	~TileMap() = default;
	void Init();
	void Update(int DirX, int DirY);
	void Clear();
};
#endif
