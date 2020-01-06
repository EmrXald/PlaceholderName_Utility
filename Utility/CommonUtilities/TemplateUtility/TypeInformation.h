#pragma once
#include "CompileTimeTypeInformation.h"
#include "RuntimeTypeInformation.h"

//#define USE_COMPILETIME_TYPE_ID
namespace TemplateUtility
{
	using TypeID = RTTI::RuntimeTypeIDType;

	template<class T>
	inline TypeID GetTypeID()
	{
#ifdef USE_COMPILETIME_TYPE_ID
		return CTTI::Cexpr_TypeID<T>();
#else
		return RTTI::GetRuntimeTypeID<T>();
#endif // USE_COMPILETIME_TYPE_ID
	}

	template<class T>
	inline const char* GetTypeName()
	{
#ifdef USE_COMPILETIME_TYPE_ID
		return CTTI::Cexpr_TypeName<T>();
#else
		return RTTI::GetTypeName<T>();
#endif // USE_COMPILETIME_TYPE_ID
	}

}

namespace TU = TemplateUtility;