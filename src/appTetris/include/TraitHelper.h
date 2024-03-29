#ifndef TRAITHELPER_H
#define TRAITHELPER_H

#ifndef TYPETRAIT_H
#define TYPETRAIT_H
#include <type_traits>
#endif

#ifndef UMAP_H
#define UMAP_H
#include <unordered_map>
#endif

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdlib>
#endif

/// <summary>
/// Custom Hash lookup for using Enum as TKey with STL Hash Collection
/// </summary>
struct EnumHash
{
	template<typename T>
	std::size_t operator()(const T& Val) const
	{
		return (std::size_t)(const_cast<T>(Val));
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
