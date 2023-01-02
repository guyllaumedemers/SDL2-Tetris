#ifndef INCLUDED_CUSTOM_TRAITS
#define INCLUDED_CUSTOM_TRAITS

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#ifndef INCLUDED_CSTD_LIB
#define INCLUDED_CSTD_LIB
#include <cstdlib>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#ifndef INCLUDED_TYPE_TRAIT
#define INCLUDED_TYPE_TRAIT
#include <type_traits>
#endif

#include <iostream>

/// <summary>
/// Custom Hash lookup for using Enum as TKey with STL Hash Collection
/// </summary>
struct EnumHash
{
	template<typename T>
	std::size_t operator()(const T& Val) const
	{
		bool Result = (std::size_t)(const_cast<T>(Val));
		std::cout << "Hash: " << Result << std::endl;
		return Result;
	}
};

template<typename TKey>
using ConditionalHash = std::conditional_t<std::is_enum_v<TKey>, EnumHash, std::hash<TKey>>;

/// <summary>
/// Custom Equality lookup for using Enum as TKey with STL Hash Collection
/// </summary>
struct EnumEquality
{
	template<typename T>
	constexpr bool operator()(const T& lhs, const T& rhs) const
	{
		auto lhsHash = (std::size_t)(const_cast<T>(lhs));
		auto rhsHash = (std::size_t)(const_cast<T>(rhs));
		std::cout << "Equal1: " << lhsHash << " Equal2: " << rhsHash << std::endl;
		return (bool)((int)(const_cast<T>(lhs)) & (int)(const_cast<T>(rhs)));
	}
};

template<typename TKey>
using ConditionalEquality = std::conditional_t<std::is_enum_v<TKey>, EnumEquality, std::equal_to<TKey>>;

/// <summary>
/// Custom UnorderedMap Collection with unique hash-equality lookup
/// </summary>
/// <typeparam name="TKey"></typeparam>
/// <typeparam name="TVal"></typeparam>
template<typename TKey, typename TVal>
using UnorderedMap = std::unordered_map<TKey, TVal, ConditionalHash<TKey>, ConditionalEquality<TKey>>;
#endif
