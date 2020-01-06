#pragma once

namespace TemplateUtility
{
	template <size_t N, typename... T>
	struct TypeArray;

	template <size_t N, typename H, typename... T>
	struct TypeArray <N, H, T...> : TypeArray <N - 1, T...> { };

	template <typename H, typename... T>
	struct TypeArray <0, H, T...> { using Type = H; };

	template<size_t N, class ... TypeList>
	using ChooseType = typename TypeArray<N, TypeList ...>::Type;
}

namespace TU = TemplateUtility;