#pragma once
namespace TemplateUtility
{
	template<class Family, class IteratorType = unsigned int>
	class TypeFamily
	{
	public:
		using family_type = IteratorType;
	private:
		inline static family_type ourMemberIterator = 0;

		template<class ...>
		inline static const auto memberID = ourMemberIterator++;
	public:
		template<class ... FamilyMember>
		inline static const family_type type = memberID<std::decay_t<FamilyMember>...>;
	};
}
namespace TU = TemplateUtility;