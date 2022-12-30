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
std::shared_ptr<TetrominoeWallKickHelper> TetrominoeWallKickHelper::Singleton = nullptr;
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
};

#pragma warning (push)
#pragma warning (disable : 4172)

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

		auto Iterator = WallKickRealignmentMap.find(static_cast<bool>(JLTSZ & TetrominoePtrArg->GetTetrominoeShape()) ? JLTSZ : I);
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

#pragma warning (pop)

TetrominoeWallKickHelper* TetrominoeWallKickHelper::Get()
{
	/// <summary>
	/// cannot use make_shared or make_unique with private constructor level
	/// </summary>
	/// <returns></returns>
	if (!Singleton)
	{
		Singleton.reset(new TetrominoeWallKickHelper());
	}
	return Singleton.get();
}
