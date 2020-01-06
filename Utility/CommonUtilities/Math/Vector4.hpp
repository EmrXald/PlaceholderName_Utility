#pragma once
#include <assert.h>
namespace CommonUtility
{
	template<class T>
	class Vector4
	{
	public:
			T x;
			T y;
			T z;
			T w;

		//Creates a null-vector
		Vector4<T>();

		//Creates a vector (aX, aY, aZ)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);

		//Copy constructor (compiler generated)
		Vector4<T>(const Vector4<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector4<T>&operator=(const Vector4<T>& aVector4) = default;

		//Destructor (compiler generated)
		~Vector4<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector4<T>GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;

		//Will calculate the cross product, working with the vectors as if they were 3D, as the cross product does not exist in 4D space.
		Vector4<T> Cross(const Vector4<T>& aVector) const;

		static Vector4<T> GetCommonColor() { return Vector4<T>(0.9f, 0.8f, 0.7f, 1.f); }

	};

	//Returns the vector sum of aVector0 and aVector1
	template<class T>Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
	}

	//Returns the vector difference of aVector0 and aVector1
	template<class T>Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector4<T> operator*(const Vector4<T>&aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y* aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector4<T> operator*(const T &aScalar, const Vector4<T>& aVector)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y* aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)
	template<class T>Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		return Vector4<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template<class T>void operator+=(Vector4<T>&aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template<class T>void operator-=(Vector4<T>&aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template<class T>void operator*=(Vector4<T>&aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template<class T>void operator/=(Vector4<T>&aVector, const T& aScalar)
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
	}

	template<class T>
	inline Vector4<T>::Vector4()
	{
		x = NULL;
		y = NULL;
		z = NULL;
		w = NULL;
	}

	template<class T>
	inline Vector4<T>::Vector4(const T & aX, const T & aY, const T & aZ, const T & aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template<class T>
	inline T Vector4<T>::LengthSqr() const
	{
		return (x*x + y*y + z*z + w*w);
	}

	template<class T>
	inline T Vector4<T>::Length() const
	{
		return static_cast<T>(std::sqrt(x*x + y * y + z * z + w * w));
	}

	template<class T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		T length = Length();
		return Vector4<T>(x / length, y / length, z / length, w / length);
	}

	template<class T>
	inline void Vector4<T>::Normalize()
	{
		if (LengthSqr() == static_cast<T>(0.0f))
		{
			return;
		}

		T temp = static_cast<T>(sqrt(x*x + y * y + z * z + w * w));
		x /= temp;
		y /= temp;
		z /= temp;
		w /= temp;
	}

	template<class T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x  * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	template<class T>
	inline Vector4<T> Vector4<T>::Cross(const Vector4<T>& aVector) const
	{
		return Vector4<T>((y * aVector.z) - (aVector.y * z), (z * aVector.x) - (x * aVector.z), (x * aVector.y) - (y * aVector.x), 0);
	}

	using Vector4f = Vector4<float>;
	using Color = Vector4f;

}
namespace CU = CommonUtility;

