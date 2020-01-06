#pragma once
#include "TypeFamily.h"
#include <typeinfo>

/*WIP Runtime Type Information interface. generates Type IDs at runtime, meaning a given type might have two different IDs from each execution(?).*/

class AbstractTypeID;
template<class T>
class TypeIDHandle;

namespace RTTI
{
	enum class RTTIEnum;
	using RuntimeTypeFamily = TemplateUtility::TypeFamily<RTTIEnum>;
	using RuntimeTypeIDType = RuntimeTypeFamily::family_type;

	template<class T>
	inline RuntimeTypeIDType GetRuntimeTypeID()
	{
		return RuntimeTypeFamily::template type<T>;
	}

	template<class T>
	inline const char* GetTypeName()
	{
		return typeid(T).name();
	}
}