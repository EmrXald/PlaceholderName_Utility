#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace CommonUtility
{
	template<class T>
	Vector3<T> ToVector3(const Vector4<T>& aVec4);
	template<class T>
	Vector4<T> ToVector4(const Vector3<T>& aVec3, const T& aW);

	template<class T>
	Vector3<T> ToVector3(const Vector4<T>& aVec4)
	{
		return{ aVec4.x, aVec4.y, aVec4.z };
	}

	template<class T>
	Vector4<T> ToVector4(const Vector3<T>& aVec3, const T& aW)
	{
		return{ aVec3.x, aVec3.y, aVec3.z , aW};
	}

}
namespace CU = CommonUtility;