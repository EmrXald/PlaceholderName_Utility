#pragma once
#include "Matrix4x4.hpp"
#include <math.h>
#include "Vector3.hpp"
#include "DL_Assert.h"

namespace CommonUtility
{
	template<class T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		Matrix3x3<T>(const T(&anArray)[3][3]);
		Matrix3x3<T>(std::initializer_list<T> a2DList);

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);
		static T Determinant(const Matrix3x3<T>& aMatrix);
		static Matrix3x3<T> Inverse(const Matrix3x3<T>& aMatrixToInvert);


		void operator+=(const Matrix3x3<T>& aMatrix);
		void operator-=(const Matrix3x3<T>& aMatrix);
		void operator*=(const Matrix3x3<T>& aMatrix);
		void operator*=(const T aScalar);
		void operator=(const Matrix3x3<T>& aMatrix);
		T& operator()(int aRow, int aCol);
		const T& operator()(int aRow, int aCol) const;

	private:
		T myElements[3][3];
	};
	template<class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix);

	template<class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix);

	template<class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix);

	template<class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix);

	template<class T>
	Matrix3x3<T> operator*(const T aScalar, const Matrix3x3<T>& aMatrix);

	template<class T>
	bool operator==(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix);

	template<class T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] = static_cast<T>(0);
			}
		}
		myElements[0][0] = static_cast<T>(1);
		myElements[1][1] = static_cast<T>(1);
		myElements[2][2] = static_cast<T>(1);
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] = aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const T(&anArray)[3][3])
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				myElements[j][i] = anArray[j][i];
			}
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] = aMatrix(j + 1, i + 1);
			}
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3<T>(std::initializer_list<T> anInitList)
	{
		assert(anInitList.size() == 9 && "Init list does not have 3 rows.");
		auto begin = anInitList.begin();
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				myElements[i][j] = *(begin + j + (i * 3));
			}
		}
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> retMatrix;

		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				retMatrix.myElements[j][i] = aMatrixToTranspose.myElements[i][j];
			}
		}
		return retMatrix;
	}

	template<class T>
	inline T Matrix3x3<T>::Determinant(const Matrix3x3<T>& aMatrix)
	{
		return(aMatrix(1, 1) * (aMatrix(2, 2) * aMatrix(3, 3) - aMatrix(3, 2) * aMatrix(2, 3)) -
               aMatrix(1, 2) * (aMatrix(2, 1) * aMatrix(3, 3) - aMatrix(2, 3) * aMatrix(3, 2)) +
               aMatrix(1, 3) * (aMatrix(2, 1) * aMatrix(3, 2) - aMatrix(2, 2) * aMatrix(3, 2)));
	}

	/*
	[0] = [0][0]
	[1] = [0][1]
	[2] = [0][2]
	[3] = [1][0]
	[4] = [1][1]
	[5] = [1][2]
	[6] = [2][0]
	[7] = [2][1]
	[8] = [2][2]
	*/

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Inverse(const Matrix3x3<T>& aMatrixToInvert)
	{
		T det = Determinant(aMatrixToInvert);		
		assert(det != 0 && "Non-invertible matrix");

		T invDet = static_cast<T>(1) / (det);
		
		Matrix3x3<T> minv;
		minv(1, 1) = (aMatrixToInvert(2, 2) * aMatrixToInvert(3, 3) - aMatrixToInvert(3, 2) * aMatrixToInvert(2, 3)) * invDet;
		minv(1, 2) = (aMatrixToInvert(1, 3) * aMatrixToInvert(3, 2) - aMatrixToInvert(1, 2) * aMatrixToInvert(3, 3)) * invDet;
		minv(1, 3) = (aMatrixToInvert(1, 2) * aMatrixToInvert(2, 3) - aMatrixToInvert(1, 3) * aMatrixToInvert(2, 2)) * invDet;
		minv(2, 1) = (aMatrixToInvert(2, 3) * aMatrixToInvert(3, 1) - aMatrixToInvert(2, 1) * aMatrixToInvert(3, 3)) * invDet;
		minv(2, 2) = (aMatrixToInvert(1, 1) * aMatrixToInvert(3, 3) - aMatrixToInvert(1, 3) * aMatrixToInvert(3, 1)) * invDet;
		minv(2, 3) = (aMatrixToInvert(2, 1) * aMatrixToInvert(1, 3) - aMatrixToInvert(1, 1) * aMatrixToInvert(2, 3)) * invDet;
		minv(3, 1) = (aMatrixToInvert(2, 1) * aMatrixToInvert(3, 2) - aMatrixToInvert(3, 1) * aMatrixToInvert(2, 2)) * invDet;
		minv(3, 2) = (aMatrixToInvert(3, 1) * aMatrixToInvert(1, 2) - aMatrixToInvert(1, 1) * aMatrixToInvert(3, 2)) * invDet;
		minv(3, 3) = (aMatrixToInvert(1, 1) * aMatrixToInvert(2, 2) - aMatrixToInvert(2, 1) * aMatrixToInvert(1, 2)) * invDet;
		
		return minv;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp.myElements[1][1] = cos(aAngleInRadians);
		temp.myElements[1][2] = sin(aAngleInRadians);
		temp.myElements[2][1] = -sin(aAngleInRadians);
		temp.myElements[2][2] = cos(aAngleInRadians);
		return temp;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp.myElements[0][0] = cos(aAngleInRadians);
		temp.myElements[0][2] = -sin(aAngleInRadians);
		temp.myElements[2][0] = sin(aAngleInRadians);
		temp.myElements[2][2] = cos(aAngleInRadians);

		return temp;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp.myElements[0][0] = cos(aAngleInRadians);
		temp.myElements[0][1] = sin(aAngleInRadians);
		temp.myElements[1][0] = -sin(aAngleInRadians);
		temp.myElements[1][1] = cos(aAngleInRadians);

		return temp;
	}

	template<class T>
	inline void Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] += aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline void Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] -= aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline void Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> temp(*this);

		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] =
					temp.myElements[j][0] * aMatrix.myElements[0][i] +
					temp.myElements[j][1] * aMatrix.myElements[1][i] +
					temp.myElements[j][2] * aMatrix.myElements[2][i];
			}
		}
	}

	template<class T>
	inline void Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myElements[j][i] = aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline T& Matrix3x3<T>::operator()(int aRow, int aCol)
	{
		assert(aRow > 0 && aRow < 4 && "Bad row index, remember it is indexed from 1.");
		assert(aCol > 0 && aCol < 4 && "Bad column index, remember it is indexed from 1.");
		return myElements[aRow - 1][aCol - 1];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator()(int aRow, int aCol) const
	{
		assert(aRow > 0 && aRow < 4 && "Bad row index, remember it is indexed from 1.");
		assert(aCol > 0 && aCol < 4 && "Bad column index, remember it is indexed from 1.");
		return myElements[aRow - 1][aCol - 1];
	}

	template<class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix)
	{
		Matrix3x3<T> temp(aMatrix);
		temp += aSecondMatrix;
		return temp;
	}

	template<class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix)
	{
		Matrix3x3<T> temp(aMatrix);
		temp -= aSecondMatrix;
		return temp;
	}

	template<class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix)
	{
		Matrix3x3<T> temp(aMatrix);
		temp *= aSecondMatrix;
		return temp;
	}

	template<class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T>temp(aVector);

		temp.x = (aVector.x * aMatrix(1,1)) +
			(aVector.y * aMatrix(2,1)) +
			(aVector.z * aMatrix(3,1));

		temp.y = (aVector.x * aMatrix(1,2)) +
			(aVector.y * aMatrix(2,2)) +
			(aVector.z * aMatrix(3,2));

		temp.z = (aVector.x * aMatrix(1,3)) +
			(aVector.y * aMatrix(2,3)) +
			(aVector.z * aMatrix(3,3));

		return temp;
	}

	template<class T>
	Matrix3x3<T> operator*(const T aScalar, const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> temp(aMatrix);
		temp *= aScalar;

		return temp;
	}

	template<class T>
	bool operator==(const Matrix3x3<T>& aMatrix, const Matrix3x3<T>& aSecondMatrix)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (aMatrix(j + 1, i + 1) != aSecondMatrix(j + 1, i + 1))
				{
					return false;
				}

			}
		}
		return true;
	}
	template<class T>
	inline void Matrix3x3<T>::operator*=(const T aScalar)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				myElements[j][i] *= aScalar;
			}
		}
	}

	using Matrix3x3f = Matrix3x3<float>;
}
namespace CU = CommonUtility;
