#include "../include/TetrominoeWallKickHelper.h"
#include "../include/Tetrominoe.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
#endif

#ifndef INCLUDED_ALGORITHM
#define INCLUDED_ALGORITHM
#include <algorithm>
#endif

// --- Static Fields
std::unique_ptr<TetrominoeWallKickHelper> TetrominoeWallKickHelper::Singleton = nullptr;
// ---

TetrominoeWallKickHelper::TetrominoeWallKickHelper()
{
	// --- Group
	JLTSZ |= TetrominoeShapeEnum::JShape;
	JLTSZ |= TetrominoeShapeEnum::LShape;
	JLTSZ |= TetrominoeShapeEnum::TShape;
	JLTSZ |= TetrominoeShapeEnum::SShape;
	JLTSZ |= TetrominoeShapeEnum::ZShape;
	// ---
	I |= TetrominoeShapeEnum::IShape;

	WallKickRealignmentMap = std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>, std::hash<TetrominoeShapeEnum>>
	{
		std::make_pair<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>>(
			JLTSZ,
			std::vector<TetrominoeWallKicks>
			{
				TetrominoeWallKicks(),
				TetrominoeWallKicks(),
				TetrominoeWallKicks(),
				TetrominoeWallKicks()
			}),

		std::make_pair<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>>(
			I,
			std::vector<TetrominoeWallKicks>
			{
				TetrominoeWallKicks(),
				TetrominoeWallKicks(),
				TetrominoeWallKicks(),
				TetrominoeWallKicks()
			}),
	};
};

const TetrominoeWallKicks& TetrominoeWallKickHelper::TryWallKickRealignment(Tetrominoe* TetrominoePtrArg)
{
	if (WallKickRealignmentMap.empty())
	{
		return TetrominoeWallKicks();
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return TetrominoeWallKicks();
		}

		auto Iterator = std::find(WallKickRealignmentMap.begin(), WallKickRealignmentMap.end(), (JLTSZ & TetrominoePtrArg->GetTetrominoeShape()) ? JLTSZ : I);
		if (Iterator != WallKickRealignmentMap.end())
		{
			return Iterator->second.at(TetrominoePtrArg->GetTetrominoeRotationIndex());
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY REALIGNMENTOUTCOME AT ROTATION INDEX FUNCTION! %s", e.what());
	}
	return TetrominoeWallKicks();
}

TetrominoeWallKickHelper* TetrominoeWallKickHelper::Get()
{
	return (!Singleton) ? (Singleton = std::make_unique<TetrominoeWallKickHelper>()).get() : Singleton.get();
}
