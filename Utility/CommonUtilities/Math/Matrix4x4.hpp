#pragma once
#include <cstring>
#include <initializer_list>
#include <math.h>
#include "Vector4.hpp"
#include <assert.h>

namespace CommonUtility
{
	template<class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		Matrix4x4(const T(&anArray)[4][4]);
		Matrix4x4<T>(std::initializer_list<T> a2DList);

		void ConstructOrientation(const Vector4<T>& aBasise0, const Vector4<T>& aBasise1, const Vector4<T>& aBasise2);
		
		Vector4<T> GetEulerAngles();

		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aMatrixToInverse);
		static Matrix4x4<T> GetRealInverse(const Matrix4x4<T>& aMatrixToInverse);
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		static Matrix4x4<T> CreateScaleMatrix(const Vector4<T>& aScaleVec);

		void operator+=(const Matrix4x4<T>& aMatrix);
		void operator-=(const Matrix4x4<T>& aMatrix);
		void operator*=(const Matrix4x4<T>& aMatrix);
		void operator*=(const T aScalar);
		void operator=(const Matrix4x4<T>& aMatrix);
		T& operator()(int aRow, int aCol);
		const T& operator()(int aRow, int aCol) const;

	private:
		T myElements[4][4];
	};
	template<class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix);

	template<class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix);

	template<class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix);

	template<class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix);

	template<class T>
	Matrix4x4<T> operator*(const T aScalar, const Matrix4x4<T>& aMatrix);

	template<class T>
	bool operator==(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix);

	template<class T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] = static_cast<T>(0);
			}
		}
		myElements[0][0] = static_cast<T>(1);
		myElements[1][1] = static_cast<T>(1);
		myElements[2][2] = static_cast<T>(1);
		myElements[3][3] = static_cast<T>(1);
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] = aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(std::initializer_list<T> anInitList)
	{
		assert(anInitList.size() == 16 && "Init list does not have 4 rows.");
		auto begin = anInitList.begin();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				myElements[i][j] = *(begin + j + (i * 4));
			}
		}
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const T(&anArray)[4][4])
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				myElements[j][i] = anArray[j][i];
			}
		}
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> retMatrix;

		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				retMatrix.myElements[j][i] = aMatrixToTranspose.myElements[i][j];
			}
		}
		return retMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		temp.myElements[1][1] = static_cast<T>(cos(aAngleInRadians));
		temp.myElements[1][2] = static_cast<T>(sin(aAngleInRadians));
		temp.myElements[2][1] = static_cast<T>(-sin(aAngleInRadians));
		temp.myElements[2][2] = static_cast<T>(cos(aAngleInRadians));
		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		temp.myElements[0][0] = static_cast<T>(cos(aAngleInRadians));
		temp.myElements[0][2] = static_cast<T>(-sin(aAngleInRadians));
		temp.myElements[2][0] = static_cast<T>(sin(aAngleInRadians));
		temp.myElements[2][2] = static_cast<T>(cos(aAngleInRadians));

		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		temp.myElements[0][0] = static_cast<T>(cos(aAngleInRadians));
		temp.myElements[0][1] = static_cast<T>(sin(aAngleInRadians));
		temp.myElements[1][0] = static_cast<T>(-sin(aAngleInRadians));
		temp.myElements[1][1] = static_cast<T>(cos(aAngleInRadians));

		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector4<T>& aScaleVec)
	{
		return Matrix4x4<T>({ { aScaleVec.x * aScaleVec.w, 0, 0, 0 }, { 0, aScaleVec.y * aScaleVec.w, 0, 0 }, { 0, 0, aScaleVec.z * aScaleVec.w, 0 }, { 0, 0, 0, 1 } });
	}

	template<class T>
	inline void Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] += aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline void Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] -= aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline void Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> temp(*this);

		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] =
					temp.myElements[j][0] * aMatrix.myElements[0][i] +
					temp.myElements[j][1] * aMatrix.myElements[1][i] +
					temp.myElements[j][2] * aMatrix.myElements[2][i] +
					temp.myElements[j][3] * aMatrix.myElements[3][i];
			}
		}
	}

	template<class T>
	inline void Matrix4x4<T>::operator*=(const T aScalar)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] *= aScalar;
			}
		}
	}

	template<class T>
	inline void Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				myElements[j][i] = aMatrix.myElements[j][i];
			}
		}
	}

	template<class T>
	inline T& Matrix4x4<T>::operator()(int aRow, int aCol)
	{
		assert(aRow > 0 && aRow < 5 && "Bad row index, remember it is indexed from 1.");
		assert(aCol > 0 && aCol < 5 && "Bad column index, remember it is indexed from 1.");
		return myElements[aRow - 1][aCol - 1];
	}

	template<class T>
	inline const T& Matrix4x4<T>::operator()(int aRow, int aCol) const
	{
		assert(aRow > 0 && aRow < 5 && "Bad row index, remember it is indexed from 1.");
		assert(aCol > 0 && aCol < 5 && "Bad column index, remember it is indexed from 1.");
		return myElements[aRow - 1][aCol - 1];
	}

	template<class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp(aMatrix);
		temp += aSecondMatrix;
		return temp;
	}

	template<class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp(aMatrix);
		temp -= aSecondMatrix;
		return temp;
	}

	template<class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp(aMatrix);
		temp *= aSecondMatrix;
		return temp;
	}

	template<class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T>temp(aVector);

		temp.x = (aVector.x * aMatrix(1,1)) +
			(aVector.y * aMatrix(2, 1)) +
			(aVector.z * aMatrix(3, 1)) +
			(aVector.w * aMatrix(4, 1));

		temp.y = (aVector.x * aMatrix(1, 2)) +
			(aVector.y * aMatrix(2, 2)) +
			(aVector.z * aMatrix(3, 2)) +
			(aVector.w * aMatrix(4, 2));

		temp.z = (aVector.x * aMatrix(1, 3)) +
			(aVector.y * aMatrix(2, 3)) +
			(aVector.z * aMatrix(3, 3)) +
			(aVector.w * aMatrix(4, 3));

		temp.w = (aVector.x * aMatrix(1, 4)) +
			(aVector.y * aMatrix(2, 4)) +
			(aVector.z * aMatrix(3, 4)) +
			(aVector.w * aMatrix(4, 4));

		return temp;
	}

	template<class T>
	Matrix4x4<T> operator*(const T aScalar, const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> temp(aMatrix);
		temp *= aScalar;

		return temp;
	}

	template<class T>
	bool operator==(const Matrix4x4<T>& aMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		float threshhold = 1e-3f;
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				if (fabsf(aMatrix(j + 1, i + 1) - aSecondMatrix(j + 1, i + 1)) > threshhold)
				{
					return false;
				}

			}
		}
		return true;
	}

	template<class T>
	inline void Matrix4x4<T>::ConstructOrientation(const Vector4<T> & aBasise0, const Vector4<T> & aBasise1, const Vector4<T> & aBasise2)
	{
		memcpy(&myElements[0][0],&aBasise0, sizeof(T) * 4);
		memcpy(&myElements[1][0], &aBasise1, sizeof(T) * 4);
		memcpy(&myElements[2][0], &aBasise2, sizeof(T) * 4);
	}

	template<class T>
	inline Vector4<T> Matrix4x4<T>::GetEulerAngles()
	{
		float sy = sqrtf(myElements[0][0] * myElements[0][0] + myElements[1][0] * myElements[1][0]);

		bool singular = sy < 1e-6f;

		Vector4<T> eulerAng;

		if (!singular)
		{
			eulerAng.x = static_cast<T>(atan2( myElements[2][1], myElements[2][2]));
			eulerAng.y = static_cast<T>(atan2(-myElements[2][0], sy));
			eulerAng.z = static_cast<T>(atan2( myElements[1][0], myElements[0][0]));
		}
		else
		{
			eulerAng.x = static_cast<T>(atan2(-myElements[1][2], myElements[1][1]));
			eulerAng.y = static_cast<T>(atan2(-myElements[2][0], sy));
			eulerAng.z = 0;
		}

		return eulerAng;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aMatrixToInverse)
	{
		Matrix4x4<T> retMatrix;
		Vector4<T> translationVec = { -aMatrixToInverse(4,1),-aMatrixToInverse(4,2) ,-aMatrixToInverse(4,3) , static_cast<T>(0) };
		retMatrix = Matrix4x4<T>::Transpose(aMatrixToInverse);
		retMatrix(1, 4) = static_cast<T>(0);
		retMatrix(2, 4) = static_cast<T>(0);
		retMatrix(3, 4) = static_cast<T>(0);
		retMatrix(4, 4) = static_cast<T>(0);
		translationVec = translationVec * retMatrix;

		retMatrix(4, 1) = translationVec.x;
		retMatrix(4, 2) = translationVec.y;
		retMatrix(4, 3) = translationVec.z;
		retMatrix(4, 4) = static_cast<T>(1);

		return retMatrix;
	}
	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetRealInverse(const Matrix4x4<T>& aMatrixToInverse)
	{
		Matrix4x4<T> retMatrix;
		aMatrixToInverse;
		//start turn [4][4] into [16]
		T matrixToInverse[16], determinant;
		determinant;
		int j = 0;
		for (unsigned int index = 0; index < 16; ++index)
		{
			unsigned int i = index % 4;
			matrixToInverse[index] = aMatrixToInverse(j+1,i+1);					//switch this if x and y is wrong(also when u revert back)

			if (i == 3)
				++j;
		}
		//end turn [4][4] into [16]

		//start real inverse magic
		//https://www.gamedev.net/forums/topic/648190-algorithm-for-4x4-matrix-inverse/
		T inv[16],invOut[16];

		inv[0] = matrixToInverse[5] * matrixToInverse[10] * matrixToInverse[15] -
			matrixToInverse[5] * matrixToInverse[11] * matrixToInverse[14] -
			matrixToInverse[9] * matrixToInverse[6] * matrixToInverse[15] +
			matrixToInverse[9] * matrixToInverse[7] * matrixToInverse[14] +
			matrixToInverse[13] * matrixToInverse[6] * matrixToInverse[11] -
			matrixToInverse[13] * matrixToInverse[7] * matrixToInverse[10];

		inv[4] = -matrixToInverse[4] * matrixToInverse[10] * matrixToInverse[15] +
			matrixToInverse[4] * matrixToInverse[11] * matrixToInverse[14] +
			matrixToInverse[8] * matrixToInverse[6] * matrixToInverse[15] -
			matrixToInverse[8] * matrixToInverse[7] * matrixToInverse[14] -
			matrixToInverse[12] * matrixToInverse[6] * matrixToInverse[11] +
			matrixToInverse[12] * matrixToInverse[7] * matrixToInverse[10];

		inv[8] = matrixToInverse[4] * matrixToInverse[9] * matrixToInverse[15] -
			matrixToInverse[4] * matrixToInverse[11] * matrixToInverse[13] -
			matrixToInverse[8] * matrixToInverse[5] * matrixToInverse[15] +
			matrixToInverse[8] * matrixToInverse[7] * matrixToInverse[13] +
			matrixToInverse[12] * matrixToInverse[5] * matrixToInverse[11] -
			matrixToInverse[12] * matrixToInverse[7] * matrixToInverse[9];

		inv[12] = -matrixToInverse[4] * matrixToInverse[9] * matrixToInverse[14] +
			matrixToInverse[4] * matrixToInverse[10] * matrixToInverse[13] +
			matrixToInverse[8] * matrixToInverse[5] * matrixToInverse[14] -
			matrixToInverse[8] * matrixToInverse[6] * matrixToInverse[13] -
			matrixToInverse[12] * matrixToInverse[5] * matrixToInverse[10] +
			matrixToInverse[12] * matrixToInverse[6] * matrixToInverse[9];

		inv[1] = -matrixToInverse[1] * matrixToInverse[10] * matrixToInverse[15] +
			matrixToInverse[1] * matrixToInverse[11] * matrixToInverse[14] +
			matrixToInverse[9] * matrixToInverse[2] * matrixToInverse[15] -
			matrixToInverse[9] * matrixToInverse[3] * matrixToInverse[14] -
			matrixToInverse[13] * matrixToInverse[2] * matrixToInverse[11] +
			matrixToInverse[13] * matrixToInverse[3] * matrixToInverse[10];

		inv[5] = matrixToInverse[0] * matrixToInverse[10] * matrixToInverse[15] -
			matrixToInverse[0] * matrixToInverse[11] * matrixToInverse[14] -
			matrixToInverse[8] * matrixToInverse[2] * matrixToInverse[15] +
			matrixToInverse[8] * matrixToInverse[3] * matrixToInverse[14] +
			matrixToInverse[12] * matrixToInverse[2] * matrixToInverse[11] -
			matrixToInverse[12] * matrixToInverse[3] * matrixToInverse[10];

		inv[9] = -matrixToInverse[0] * matrixToInverse[9] * matrixToInverse[15] +
			matrixToInverse[0] * matrixToInverse[11] * matrixToInverse[13] +
			matrixToInverse[8] * matrixToInverse[1] * matrixToInverse[15] -
			matrixToInverse[8] * matrixToInverse[3] * matrixToInverse[13] -
			matrixToInverse[12] * matrixToInverse[1] * matrixToInverse[11] +
			matrixToInverse[12] * matrixToInverse[3] * matrixToInverse[9];

		inv[13] = matrixToInverse[0] * matrixToInverse[9] * matrixToInverse[14] -
			matrixToInverse[0] * matrixToInverse[10] * matrixToInverse[13] -
			matrixToInverse[8] * matrixToInverse[1] * matrixToInverse[14] +
			matrixToInverse[8] * matrixToInverse[2] * matrixToInverse[13] +
			matrixToInverse[12] * matrixToInverse[1] * matrixToInverse[10] -
			matrixToInverse[12] * matrixToInverse[2] * matrixToInverse[9];

		inv[2] = matrixToInverse[1] * matrixToInverse[6] * matrixToInverse[15] -
			matrixToInverse[1] * matrixToInverse[7] * matrixToInverse[14] -
			matrixToInverse[5] * matrixToInverse[2] * matrixToInverse[15] +
			matrixToInverse[5] * matrixToInverse[3] * matrixToInverse[14] +
			matrixToInverse[13] * matrixToInverse[2] * matrixToInverse[7] -
			matrixToInverse[13] * matrixToInverse[3] * matrixToInverse[6];

		inv[6] = -matrixToInverse[0] * matrixToInverse[6] * matrixToInverse[15] +
			matrixToInverse[0] * matrixToInverse[7] * matrixToInverse[14] +
			matrixToInverse[4] * matrixToInverse[2] * matrixToInverse[15] -
			matrixToInverse[4] * matrixToInverse[3] * matrixToInverse[14] -
			matrixToInverse[12] * matrixToInverse[2] * matrixToInverse[7] +
			matrixToInverse[12] * matrixToInverse[3] * matrixToInverse[6];

		inv[10] = matrixToInverse[0] * matrixToInverse[5] * matrixToInverse[15] -
			matrixToInverse[0] * matrixToInverse[7] * matrixToInverse[13] -
			matrixToInverse[4] * matrixToInverse[1] * matrixToInverse[15] +
			matrixToInverse[4] * matrixToInverse[3] * matrixToInverse[13] +
			matrixToInverse[12] * matrixToInverse[1] * matrixToInverse[7] -
			matrixToInverse[12] * matrixToInverse[3] * matrixToInverse[5];

		inv[14] = -matrixToInverse[0] * matrixToInverse[5] * matrixToInverse[14] +
			matrixToInverse[0] * matrixToInverse[6] * matrixToInverse[13] +
			matrixToInverse[4] * matrixToInverse[1] * matrixToInverse[14] -
			matrixToInverse[4] * matrixToInverse[2] * matrixToInverse[13] -
			matrixToInverse[12] * matrixToInverse[1] * matrixToInverse[6] +
			matrixToInverse[12] * matrixToInverse[2] * matrixToInverse[5];

		inv[3] = -matrixToInverse[1] * matrixToInverse[6] * matrixToInverse[11] +
			matrixToInverse[1] * matrixToInverse[7] * matrixToInverse[10] +
			matrixToInverse[5] * matrixToInverse[2] * matrixToInverse[11] -
			matrixToInverse[5] * matrixToInverse[3] * matrixToInverse[10] -
			matrixToInverse[9] * matrixToInverse[2] * matrixToInverse[7] +
			matrixToInverse[9] * matrixToInverse[3] * matrixToInverse[6];

		inv[7] = matrixToInverse[0] * matrixToInverse[6] * matrixToInverse[11] -
			matrixToInverse[0] * matrixToInverse[7] * matrixToInverse[10] -
			matrixToInverse[4] * matrixToInverse[2] * matrixToInverse[11] +
			matrixToInverse[4] * matrixToInverse[3] * matrixToInverse[10] +
			matrixToInverse[8] * matrixToInverse[2] * matrixToInverse[7] -
			matrixToInverse[8] * matrixToInverse[3] * matrixToInverse[6];

		inv[11] = -matrixToInverse[0] * matrixToInverse[5] * matrixToInverse[11] +
			matrixToInverse[0] * matrixToInverse[7] * matrixToInverse[9] +
			matrixToInverse[4] * matrixToInverse[1] * matrixToInverse[11] -
			matrixToInverse[4] * matrixToInverse[3] * matrixToInverse[9] -
			matrixToInverse[8] * matrixToInverse[1] * matrixToInverse[7] +
			matrixToInverse[8] * matrixToInverse[3] * matrixToInverse[5];

		inv[15] = matrixToInverse[0] * matrixToInverse[5] * matrixToInverse[10] -
			matrixToInverse[0] * matrixToInverse[6] * matrixToInverse[9] -
			matrixToInverse[4] * matrixToInverse[1] * matrixToInverse[10] +
			matrixToInverse[4] * matrixToInverse[2] * matrixToInverse[9] +
			matrixToInverse[8] * matrixToInverse[1] * matrixToInverse[6] -
			matrixToInverse[8] * matrixToInverse[2] * matrixToInverse[5];

		determinant = matrixToInverse[0] * inv[0] + matrixToInverse[1] * inv[4] + matrixToInverse[2] * inv[8] + matrixToInverse[3] * inv[12];

		if (determinant == 0)
			assert("determinant was 0 in realinvserse calculation(divide by zero not allowed without supervision)");

		determinant = 1 / determinant;

		for (int i = 0; i < 16; ++i)
			invOut[i] = inv[i] * determinant;
		//end real inverse magic

		//start revert from [4][4] to [16]
		j = 0;
		for (unsigned int index = 0; index < 16; ++index)
		{
			unsigned int k = index % 4;
			retMatrix(j + 1, k + 1) = invOut[index];					//switch this if x and y is wrong

			if (k == 3)
				++j;
		}
		//end revert from [4][4] to [16]

		return retMatrix;
	}

	using Matrix4x4f = Matrix4x4<float>;
}
namespace CU = CommonUtility;