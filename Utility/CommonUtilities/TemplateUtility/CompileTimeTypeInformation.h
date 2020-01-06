#pragma once
#include <string_view>


namespace CTTI
{

	template <class T>
	constexpr auto Cexpr_TypeName()
	{
		std::string_view name, prefix, suffix;
#ifdef __clang__
		name = __PRETTY_FUNCTION__;
		prefix = "auto Cexpr_TypeName() [T = ";
		suffix = "]";
#elif defined(__GNUC__)
		name = __PRETTY_FUNCTION__;
		prefix = "constexpr auto Cexpr_TypeName() [with T = ";
		suffix = "]";
#elif defined(_MSC_VER)
		name = __FUNCSIG__;
		prefix = "auto __cdecl CTTI::Cexpr_TypeName<";
		suffix = ">(void)";
#endif
		name.remove_prefix(prefix.size());
		name.remove_suffix(suffix.size());
		return name;
	}

	using CompileHashType = unsigned long long;


	constexpr CompileHashType cexpr_fnvBasis = 14695981039346656037ull;
	constexpr CompileHashType cexpr_fnvPrime = 1099511628211ull;

	
	constexpr CompileHashType Cexpr_Fnv1aHash(size_t n, const char* aTypeString, CompileHashType hash = cexpr_fnvBasis)
	{
		return n > 0 ? Cexpr_Fnv1aHash(n - 1, aTypeString + 1, (hash ^ *aTypeString) * cexpr_fnvPrime) : hash;
	}

	template<std::size_t N>
	constexpr CompileHashType Cexpr_Fnv1aHash(const char(&array)[N])
	{
		return Cexpr_Fnv1aHash(N - 1, &array[0]);
	}

	template<class T>
	constexpr CompileHashType Cexpr_TypeID()
	{
		constexpr std::string_view cexpr_typeName = Cexpr_TypeName<T>();
		return Cexpr_Fnv1aHash(cexpr_typeName.size(), &(cexpr_typeName[0]));
	}
}
