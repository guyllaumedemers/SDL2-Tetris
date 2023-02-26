#ifndef ROTATIONHELPER_H
#define ROTATIONHELPER_H

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

#include "SDLlogHelper.h"

struct RotationalAlignmentContainer final
{
	/// <summary>
	/// Single Alignment which follows a Rotation
	/// </summary>
	struct RotationalAlignment final
	{
		int8_t x = INT8_MAX;
		int8_t y = INT8_MAX;

		bool operator==(const RotationalAlignment& rhs) const
		{
			return
				(x == rhs.x) &&
				(y == rhs.y);
		}

		constexpr bool IsValid() const { return ((x >= INT8_MIN) && (x < INT8_MAX)) && ((y >= INT8_MIN) && (y < INT8_MAX)); }
	};

	/// <summary>
	/// Realignment possible following rotation of tetrominoe which keep pivot point unchanged
	/// </summary>
	std::vector<RotationalAlignment> RotationalRealignmentData = std::vector<RotationalAlignment>();

	RotationalAlignmentContainer(std::initializer_list<RotationalAlignment> Alignments)
	{
		for (const auto& Alignment : Alignments)
		{
			RotationalRealignmentData.push_back(Alignment);
		}
	}

	RotationalAlignmentContainer() {}

	bool operator==(const RotationalAlignmentContainer& rhs) const
	{
		return
			(RotationalRealignmentData == rhs.RotationalRealignmentData);
	}

#pragma warning (push)
#pragma warning (disable : 4172)

	static inline const RotationalAlignment& TryGetRotationAlignmentAtIndex(const RotationalAlignmentContainer& Container, uint8_t Index)
	{
		static const RotationalAlignment& EmptyAlignment = RotationalAlignment();
		static const uint8_t& RealignmentOutcomes = Container.RotationalRealignmentData.size();

		if (Container.RotationalRealignmentData.empty())
		{
			return EmptyAlignment;
		}

		try
		{
			return Container.RotationalRealignmentData.at(Index % RealignmentOutcomes);
		}
		catch (const std::out_of_range& e)
		{
			SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "RotationalAlignment", e);
		}

		return EmptyAlignment;
	}

#pragma warning (pop)
};

#include "TetrominoeShapeEnum.h"
#include "TraitHelper.h"

class TetrominoeRotationRealignmentHelper final
{
	UnorderedMap<const TetrominoeShapeEnum, const RotationalAlignmentContainer&> RotationRealignmentMap = UnorderedMap<const TetrominoeShapeEnum, const RotationalAlignmentContainer&>();
	static std::unique_ptr<TetrominoeRotationRealignmentHelper> Singleton;
public:
	TetrominoeRotationRealignmentHelper(const TetrominoeRotationRealignmentHelper&) = delete;
	TetrominoeRotationRealignmentHelper(TetrominoeRotationRealignmentHelper&&) = delete;
	TetrominoeRotationRealignmentHelper();
	~TetrominoeRotationRealignmentHelper() = default;
	TetrominoeRotationRealignmentHelper& operator==(const TetrominoeRotationRealignmentHelper&) = delete;
	TetrominoeRotationRealignmentHelper& operator==(TetrominoeRotationRealignmentHelper&&) = delete;
	static const RotationalAlignmentContainer& TryGetRotationAlignmentContainer(class Tetrominoe* TetrominoePtrArg);
	const UnorderedMap<const TetrominoeShapeEnum, const RotationalAlignmentContainer&>& GetRotationRealignmentMap() const { return RotationRealignmentMap; }
private:
	// Getter/Setter
	static TetrominoeRotationRealignmentHelper* Get();
};
#endif