#pragma once
#include <assert.h>
namespace CommonUtility
{
	template<class T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		//Creates a null-vector
		Vector3<T>();

		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ);

		//Copy constructor (compiler generated)
		Vector3<T>(const Vector3<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector3<T>&operator=(const Vector3<T>& aVector3) = default;

		//Destructor (compiler generated)
		~Vector3<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector3<T>GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;

		//Returns the cross product of this and aVector
		Vector3<T>Cross(const Vector3<T>& aVector) const;
	};

	//Returns the vector sum of aVector0 and aVector1
	template<class T>Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1) 
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	//Returns the vector difference of aVector0 and aVector1
	template<class T>Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1) 
	{
		return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector3<T> operator*(const Vector3<T>&aVector, const T& aScalar) 
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y* aScalar, aVector.z * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector3<T> operator*(const T &aScalar, const Vector3<T>& aVector) 
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y* aScalar, aVector.z * aScalar);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)
	template<class T>Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		return Vector3<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template<class T>void operator+=(Vector3<T>&aVector0, const Vector3<T>& aVector1) 
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template<class T>void operator-=(Vector3<T>&aVector0, const Vector3<T>& aVector1) 
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template<class T>void operator*=(Vector3<T>&aVector, const T& aScalar) 
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template<class T>void operator/=(Vector3<T>&aVector, const T& aScalar) 
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
	}

	template<class T>
	inline Vector3<T>::Vector3()
	{
		x = NULL;
		y = NULL;
		z = NULL;
	}

	template<class T>
	inline Vector3<T>::Vector3(const T & aX, const T & aY, const T & aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template<class T>
	inline T Vector3<T>::LengthSqr() const
	{
		return (x*x + y*y + z*z);
	}

	template<class T>
	inline T Vector3<T>::Length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	template<class T>
	inline Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length = Length();
		return Vector3<T>(x / length, y / length, z / length);
	}

	template<class T>
	inline void Vector3<T>::Normalize()
	{
		if (LengthSqr() == static_cast<T>(0.0f))
		{
			return;
		}
		T temp = static_cast<T>(sqrt(x*x + y * y + z * z));
		x /= temp;
		y /= temp;
		z /= temp;
	}

	template<class T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x  * aVector.x +  y * aVector.y + z * aVector.z;
	}

	template<class T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return Vector3<T>((y * aVector.z)-(aVector.y * z),(z * aVector.x)-(x * aVector.z),(x * aVector.y)-(y * aVector.x));
	}

	using Vector3f = Vector3<float>;
}
namespace CU = CommonUtility;