#include "pch.h"
#include "Quaternion.h"
#include <cmath>
CommonUtility::Math3D::Quaternion::Quaternion()
{	
	//w is the real part of the quaternion.
	myVec.y = 0.0f;
	myVec.z = 0.0f;
	myVec.x = 0.0f;
	myVec.w = 1.0f;
}

CommonUtility::Math3D::Quaternion::Quaternion(const float aRadianAngle, const CommonUtility::Vector3f & aRotationAxis)
{
	float theta = aRadianAngle / 2.0f;
	float s = sin(theta);
	myVec.w = cos(theta);
	myVec.x = aRotationAxis.x * s;
	myVec.y = aRotationAxis.y * s;
	myVec.z = aRotationAxis.z * s;
}

CommonUtility::Math3D::Quaternion::Quaternion(const float aR, const float aI, const float aJ, const float aK)
{
	myVec.x = aI;
	myVec.y = aJ;
	myVec.z = aK;
	myVec.w = aR;
}

CommonUtility::Math3D::Quaternion::~Quaternion()
{
	myVec.x = 0.0f;
	myVec.y = 0.0f;
	myVec.z = 0.0f;
	myVec.w = 0.0f;
}

void CommonUtility::Math3D::Quaternion::Rotate(const float aRadianAngle, const CommonUtility::Vector3f & aRotationAxis)
{
	myVec = ((*this) * Quaternion(aRadianAngle, aRotationAxis)).myVec;
}

bool CommonUtility::Math3D::Quaternion::IsUnit()
{
	float tolerance = 10e-2f;
	return (abs(GetMagnitude() - 1.0f) < tolerance);
}

void CommonUtility::Math3D::Quaternion::ConstructOrientation(CommonUtility::Matrix4x4f & aOutOrientation) const
{
	aOutOrientation.ConstructOrientation(GetBasisVector1(), GetBasisVector2(), GetBasisVector3());
}

void CommonUtility::Math3D::Quaternion::Normalize()
{
	float n = sqrt((myVec.x * myVec.x) + (myVec.y * myVec.y) + (myVec.z * myVec.z) + (myVec.w * myVec.w));
	myVec.x /= n;
	myVec.y /= n;
	myVec.z /= n;
	myVec.w /= n;
}

CommonUtility::Vector4f CommonUtility::Math3D::Quaternion::GetBasisVector1() const
{
	return Vector4f(1.f - 2 * ((myVec.y * myVec.y) + (myVec.z * myVec.z)), 2 * ((myVec.x * myVec.y) - (myVec.z * myVec.w)), 2 * ((myVec.x * myVec.z) + (myVec.y * myVec.w)), 0.0f );
}

CommonUtility::Vector4f CommonUtility::Math3D::Quaternion::GetBasisVector2() const
{
	return Vector4f(2 * ((myVec.x * myVec.y)+(myVec.z * myVec.w)), 1 - 2 * ((myVec.x * myVec.x)+(myVec.z * myVec.z)), 2* ((myVec.y * myVec.z) - (myVec.x * myVec.w)), 0.0f);
}

CommonUtility::Vector4f CommonUtility::Math3D::Quaternion::GetBasisVector3() const
{
	return Vector4f(2 * ((myVec.x * myVec.z)-(myVec.y * myVec.w)), 2 *((myVec.y * myVec.z) + (myVec.x * myVec.w)), 1 - 2*((myVec.x * myVec.x) + (myVec.y * myVec.y)), 0.0f);
}

CommonUtility::Matrix4x4f CommonUtility::Math3D::Quaternion::GetOrientationMatrix() const
{
	CommonUtility::Matrix4x4f retMatrix;
	ConstructOrientation(retMatrix);
	return retMatrix;
}

CommonUtility::Math3D::Quaternion CommonUtility::Math3D::Quaternion::GetConjugate()
{
	return Quaternion(-myVec.x, -myVec.y, -myVec.z, myVec.w);
}

float CommonUtility::Math3D::Quaternion::GetMagnitude()
{
	return myVec.Length();
}

float CommonUtility::Math3D::Quaternion::GetSqrdMagnitude()
{
	return myVec.LengthSqr();
}

float CommonUtility::Math3D::Quaternion::GetReal() const
{
	return myVec.w;
}

CU::Vector3f CommonUtility::Math3D::Quaternion::GetComplex() const
{
	return CU::Vector3f(myVec.x, myVec.y, myVec.z);
}

void CommonUtility::Math3D::Quaternion::operator*=(const Quaternion & aRHS)
{
	/*
	From Wikipedias Hamilton Product:
	a = w;(Real part)
	b = x;(i part)
	c = y;(j part)
	d = z;(k part)
	*/

	Vector4f result;
	result.w = (myVec.w * aRHS.myVec.w) - (myVec.x * aRHS.myVec.x) - (myVec.y * aRHS.myVec.y) - (myVec.z * aRHS.myVec.z);
	result.x = (myVec.w * aRHS.myVec.x) + (myVec.x * aRHS.myVec.w) + (myVec.y * aRHS.myVec.z) - (myVec.z * aRHS.myVec.y);
	result.y = (myVec.w * aRHS.myVec.y) - (myVec.x * aRHS.myVec.z) + (myVec.y * aRHS.myVec.w) + (myVec.z * aRHS.myVec.x);
	result.z = (myVec.w * aRHS.myVec.z) + (myVec.x * aRHS.myVec.y) - (myVec.y * aRHS.myVec.x) + (myVec.z * aRHS.myVec.w);
	myVec = result;
}