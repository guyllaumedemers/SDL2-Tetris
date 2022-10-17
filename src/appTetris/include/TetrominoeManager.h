#ifndef INCLUDED_TETROMINOE_MANAGER
#define INCLUDED_TETROMINOE_MANAGER

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "Tetrominoe.h"

class TetrominoeManager /*Will handle Tetrominoe update and keep handle on last Active for mouvement update*/
{
	std::vector<std::unique_ptr<Tetrominoe>> TetrominoePool = std::vector<std::unique_ptr<Tetrominoe>>();

public:
	TetrominoeManager() = default;
	~TetrominoeManager() = default;
};
#endif