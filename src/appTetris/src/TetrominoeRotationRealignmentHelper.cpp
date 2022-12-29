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
std::unique_ptr<TetrominoeRotationRealignmentHelper> TetrominoeRotationRealignmentHelper::Singleton = nullptr;
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

	RotationRealignmentMap = std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>, std::hash<TetrominoeShapeEnum>>
	{
		std::make_pair<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>>(
			JLTSZ,
			std::vector<TetrominoeRotationRealignments>
			{
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments()
			}),

		std::make_pair<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>>(
			I,
			std::vector<TetrominoeRotationRealignments>
			{
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments()
			}),

		std::make_pair<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>>(
			O,
			std::vector<TetrominoeRotationRealignments>
			{
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments(),
				TetrominoeRotationRealignments()
			})
	};
}

const TetrominoeRotationRealignments& TetrominoeRotationRealignmentHelper::TryRotationRealignment(Tetrominoe* TetrominoePtrArg)
{
	if (RotationRealignmentMap.empty())
	{
		return TetrominoeRotationRealignments();
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return TetrominoeRotationRealignments();
		}

		auto Iterator = std::find(RotationRealignmentMap.begin(), RotationRealignmentMap.end(), (JLTSZ & TetrominoePtrArg->GetTetrominoeShape()) ? JLTSZ : ((I & TetrominoePtrArg->GetTetrominoeShape()) ? I : O));
		if (Iterator != RotationRealignmentMap.end())
		{
			return Iterator->second.at(TetrominoePtrArg->GetTetrominoeRotationIndex());
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY REALIGNMENTOUTCOME AT ROTATION INDEX FUNCTION! %s", e.what());
	}
	return TetrominoeRotationRealignments();
}

TetrominoeRotationRealignmentHelper* TetrominoeRotationRealignmentHelper::Get()
{
	return (!Singleton) ? (Singleton = std::make_unique<TetrominoeRotationRealignmentHelper>()).get() : Singleton.get();
}
