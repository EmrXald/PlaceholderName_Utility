#pragma once
namespace TemplateUtility
{
	template<class Base, class Derived>
	constexpr const bool Inherits()
	{
		return std::is_base_of<std::decay_t<Base>, std::decay_t<Derived>>::value;
	}
}
namespace TU = TemplateUtility;