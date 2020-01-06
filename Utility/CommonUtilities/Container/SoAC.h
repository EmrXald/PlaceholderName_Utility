#pragma once
#include <tuple>
#include <vector>
#include <functional>

using HashType = unsigned int;
using TypeIndexType = unsigned int;

namespace CommonUtility
{
	template<TypeIndexType PredicateIndex, class PredSigType, class ... TypeList>
	struct ISortLite;

	template<class ... TypeList>
	class SoAC
	{
	public:
		SoAC() {}
		~SoAC() {}

		template<class...ArgTypes>
		inline void Add(ArgTypes&& ... someArgs)
		{
			Add<0, ArgTypes...>(std::forward<ArgTypes>(someArgs)...);
		}

		inline void Resize(const size_t aVectorSize)
		{
			Resize<0>(aVectorSize);
		}

		inline void Reserve(const size_t aVectorSize)
		{
			Reserve<0>(aVectorSize);
		}

		inline void Swap(const size_t aFirstIndex, const size_t aSecondIndex)
		{
			Swap(aFirstIndex, aSecondIndex, std::make_index_sequence<sizeof...(TypeList)>{});
		}

		template<TypeIndexType TypeIndex>
		inline auto& Get(const size_t anElementIndex)
		{
			return GetTypeArray<TypeIndex>()[anElementIndex];
		}

		template<TypeIndexType TypeIndex>
		inline const auto& Get(const size_t anElementIndex) const
		{
			return GetTypeArray<TypeIndex>()[anElementIndex];
		}

		inline std::tuple<TypeList& ...> operator[](const size_t anIndex)
		{
			return GetTuple(anIndex, std::make_index_sequence<sizeof...(TypeList)>{});
		}

		inline std::tuple<const TypeList& ...> operator[](const size_t anIndex) const
		{
			return GetTuple(anIndex, std::make_index_sequence<sizeof...(TypeList)>{});
		}

		inline constexpr TypeIndexType GetTypeAmount() const
		{
			return sizeof...(TypeList);
		}

		inline size_t Size()
		{
			return GetTypeArray<0>().size();
		}

		inline size_t Size() const
		{
			return GetTypeArray<0>().size();
		}

	private:
		template<TypeIndexType PredicateIndex, class PredSigType, class ... TypeList>
		friend struct ISortLite;

		template<TypeIndexType TypeIndex, class HeadType, class...TrailTypes>
		inline void Add(HeadType&& anArg, TrailTypes&& ... someArgs)
		{
			GetTypeArray<TypeIndex>().emplace_back(std::forward<HeadType>(anArg));
			if constexpr (sizeof...(TrailTypes) > 0)
				Add<TypeIndex + 1, TrailTypes...>(std::forward<TrailTypes>(someArgs)...);
		}

		template<TypeIndexType TypeIndex>
		inline void Resize(const size_t aVectorSize)
		{
			GetTypeArray<TypeIndex>().resize(aVectorSize);
			if constexpr ((TypeIndex + 1) < sizeof...(TypeList))
				Resize<TypeIndex + 1>(aVectorSize);
		}

		template<TypeIndexType TypeIndex>
		inline void Reserve(const size_t aVectorSize)
		{
			GetTypeArray<TypeIndex>().resize(aVectorSize);
			if constexpr ((TypeIndex + 1) < sizeof...(TypeList))
				Reserve<TypeIndex + 1>(aVectorSize);
		}

		template<size_t ... IndexSequence>
		inline void Swap(const size_t aFirstIndex, const size_t aSecondIndex, const std::index_sequence<IndexSequence...>&)
		{
			(std::swap(GetTypeArray<IndexSequence>()[aFirstIndex], GetTypeArray<IndexSequence>()[aSecondIndex]), ...);
		}

		template<size_t ... IndexSequence>
		inline std::tuple<TypeList& ...> GetTuple(const size_t anIndex, const std::index_sequence<IndexSequence...>&)
		{
			return std::tuple<TypeList& ...>(Get<IndexSequence>(anIndex)...);
		}

		template<size_t ... IndexSequence>
		inline const std::tuple<const TypeList& ...> GetTuple(const size_t anIndex, const std::index_sequence<IndexSequence...>&) const
		{
			return std::tuple<const TypeList& ...>(Get<IndexSequence>(anIndex)...);
		}

		template<TypeIndexType TypeIndex>
		inline auto& GetTypeArray()
		{
			return std::get<TypeIndex>(myTypes);
		}

		template<TypeIndexType TypeIndex>
		inline const auto& GetTypeArray() const
		{
			return std::get<TypeIndex>(myTypes);
		}

		std::tuple<std::vector<TypeList>...> myTypes;
	};
}

namespace CU = CommonUtility;