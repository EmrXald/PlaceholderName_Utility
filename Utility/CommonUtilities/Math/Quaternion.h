#pragma once
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"

namespace CommonUtility
{
	namespace Math3D
	{
		class Quaternion
		{
		public:
			Quaternion();
			Quaternion(const float aRadianAngle, const CommonUtility::Vector3f& aRotationAxis);
			Quaternion(const float aR, const float aI, const float aJ, const float aK);
			~Quaternion();

			void Rotate(const float aRadianAngle, const CommonUtility::Vector3f& aRotationAxis);

			bool IsUnit();

			void ConstructOrientation(CommonUtility::Matrix4x4f& aOutOrientation) const;
			void Normalize();

			Vector4f GetBasisVector1() const;
			Vector4f GetBasisVector2() const;
			Vector4f GetBasisVector3() const;

			Matrix4x4f GetOrientationMatrix() const;

			Quaternion GetConjugate();
			float GetMagnitude();
			float GetSqrdMagnitude();

			float GetReal() const;
			CU::Vector3f GetComplex() const;
			const CU::Vector4f& GetRawData() const { return myVec; }

			void operator*=(const Quaternion& aRHS);
	
		private:
			CommonUtility::Vector4f myVec;
		};

		inline Quaternion operator*(const Quaternion& aLHS, const Quaternion& aRHS)
		{
			Quaternion res = aLHS;
			res *= aRHS;
			return res;
		}
	}
}
namespace CU = CommonUtility;