#pragma once
#include <assert.h>

namespace CommonUtility
{
	template<class T>
	class Vector2
	{
	public:

	T x;
	T y;

		//Creates a null-vector
		Vector2<T>();

		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);

		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector2<T>&operator=(const Vector2<T>& aVector2) = default;

		template<class U>
		Vector2<T>& operator=(const Vector2<U>& aVector)
		{
			x = static_cast<T>(aVector.x);
			y = static_cast<T>(aVector.y);
			return *this;
		}

		bool operator!=(const Vector2<T>& aOther) const { return x != aOther.x || y != aOther.y; }

		/*	template <class OtherType>
		Vector2<T>& operator=(const Vector2<OtherType>& aVector) { x = static_cast<T>(aVector.x); y = static_cast<T>(aVector.y);  return *this; }*/


		//Destructor (compiler generated)
		~Vector2<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector2<T>GetNormalized() const;

		//Normalizes the vector
		Vector2<T>& Normalize();



		void Set(T aX, T aY) { x = aX; y = aY; }

		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;

		Vector2<T> Normal() const { return Vector2<T>(y, -x); }
		const Vector2<T> Reflection(const Vector2<T>& aNormal) const;

		Vector2<T> operator*(const Vector2<T>& aVector) const { return Vector2<T>(x*aVector.x, y*aVector.y); }
		Vector2<T> operator/(const Vector2<T>& aVector) const { return Vector2<T>(x / aVector.x, y / aVector.y); }

		T Distance(const Vector2<T>& aPoint) const;
		T DistanceSqr(const Vector2<T>& aPoint) const;

		static Vector2<T> Up();
		static Vector2<T> Down();
		static Vector2<T> Left();
		static Vector2<T> Right();
		static Vector2<T> Zero();


	};

	template<class T>
	inline Vector2<T> Vector2<T>::Up()
	{
		return Vector2<T>(0, -1);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::Down()
	{
		return Vector2<T>(0, 1);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::Left()
	{
		return Vector2<T>(-1, 0);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::Right()
	{
		return Vector2<T>(1, 0);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::Zero()
	{
		return Vector2<T>(0, 0);
	}


	template<class T>
	T Vector2<T>::Distance(const Vector2<T>& aPoint) const
	{
		return (((*this) - aPoint).Length());
	}

	template<class T>
	inline T Vector2<T>::DistanceSqr(const Vector2<T>& aPoint) const
	{
		return  (((*this) - aPoint).LengthSqr());
	}

	//Returns true if aLhs has the same values as aRhs
	template<class T>
	inline bool operator==(const Vector2<T>& aLhs, const Vector2<T>& aRhs)
	{
		return aLhs.x == aRhs.x && aLhs.y == aRhs.y;
	}

	//Returns the vector sum of aVector0 and aVector1
	template<class T>Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	//Returns the vector difference of aVector0 and aVector1
	template<class T>Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector2<T> operator*(const Vector2<T>&aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y* aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template<class T>Vector2<T> operator*(const T &aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y* aScalar);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)
	template<class T>Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		return Vector2<T>(aVector.x / aScalar, aVector.y / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template<class T>void operator+=(Vector2<T>&aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template<class T>void operator-=(Vector2<T>&aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template<class T>void operator*=(Vector2<T>&aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template<class T>
	inline void operator*=(Vector2<T>& aLhs, const Vector2<T>& aRhs)
	{
		aLhs.x *= aRhs.x;
		aLhs.y *= aRhs.y;
	}


	//Equivalent to setting aVector to (aVector / aScalar)
	template<class T>void operator/=(Vector2<T>&aVector, const T& aScalar)
	{
		assert(aScalar != static_cast<T>(0) && "Tried to divide Vector2 with zero-value scalar.");
		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}



	template<class T>
	inline Vector2<T>::Vector2()
	{
		x = NULL;
		y = NULL;
	}

	template<class T>
	inline Vector2<T>::Vector2(const T & aX, const T & aY)
	{
		x = aX;
		y = aY;
	}

	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return (x*x + y*y);
	}

	template<class T>
	inline T Vector2<T>::Length() const
	{
		return sqrt(x*x + y*y);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();
		return Vector2<T>(x / length, y / length);
	}

	template<class T>
	inline Vector2<T>& Vector2<T>::Normalize()
	{
		if (LengthSqr() == static_cast<T>(0.0f))
		{
			return *this;
		}
		T temp = sqrt(x*x + y*y);
		x /= temp;
		y /= temp;

		return *this;
	}

	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x  * aVector.x + y * aVector.y;
	}

	template<class T>
	inline const Vector2<T> Vector2<T>::Reflection(const Vector2<T>& aNormal) const
	{
		const Vector2<T> n = aNormal.GetNormalized();
		const Vector2<T>& d = *this;
		return d - static_cast<T>(2) * (d.Dot(n)) * n;
	}

	using Vector2f = Vector2<float>;
	using Vector2ui = Vector2<unsigned int>;

}
namespace CU = CommonUtility;
