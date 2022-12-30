#include "../include/TetrominoeRotationRealignmentHelper.h"
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
std::shared_ptr<TetrominoeRotationRealignmentHelper> TetrominoeRotationRealignmentHelper::Singleton = nullptr;
// ---

TetrominoeRotationRealignmentHelper::TetrominoeRotationRealignmentHelper()
{
	// --- Group
	JLTSZ |= TetrominoeShapeEnum::JShape;
	JLTSZ |= TetrominoeShapeEnum::LShape;
	JLTSZ |= TetrominoeShapeEnum::TShape;
	JLTSZ |= TetrominoeShapeEnum::SShape;
	JLTSZ |= TetrominoeShapeEnum::ZShape;
	// ---
	I |= TetrominoeShapeEnum::IShape;
	O |= TetrominoeShapeEnum::OShape;
}

#pragma warning (push)
#pragma warning (disable : 4172)

const TetrominoeRotationRealignments& TetrominoeRotationRealignmentHelper::TryRotationRealignment(Tetrominoe* TetrominoePtrArg)
{
	/*if (RotationRealignmentMap.empty())
	{
		return TetrominoeRotationRealignments();
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return TetrominoeRotationRealignments();
		}

		auto Iterator = std::find(RotationRealignmentMap.begin(), RotationRealignmentMap.end(), static_cast<bool>(JLTSZ & TetrominoePtrArg->GetTetrominoeShape()) ? JLTSZ : (static_cast<bool>(I & TetrominoePtrArg->GetTetrominoeShape()) ? I : O));
		if (Iterator != RotationRealignmentMap.end())
		{
			return Iterator->second.at(TetrominoePtrArg->GetTetrominoeRotationIndex());
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY REALIGNMENTOUTCOME AT ROTATION INDEX FUNCTION! %s", e.what());
	}*/
	return TetrominoeRotationRealignments();
}

#pragma warning (pop)

TetrominoeRotationRealignmentHelper* TetrominoeRotationRealignmentHelper::Get()
{
	/// <summary>
	/// cannot use make_shared or make_unique with private constructor level
	/// </summary>
	/// <returns></returns>
	if (!Singleton)
	{
		Singleton.reset(new TetrominoeRotationRealignmentHelper());
	}
	return Singleton.get();
}
