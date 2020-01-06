#include "pch.h"
#include "Transform3D.h"
#include "CommonMath.h"
#define MATH_PI 3.14159265359f
#include "VectorFunctions.h"

Transform3D::Transform3D() : myPosition(0, 0, 0, 1), myRotQuaternion(), myOrientationMatrix(), myTransform()
{

}


Transform3D::~Transform3D()
{
}

void Transform3D::Init(const CommonUtility::Vector3f & aPos, const CommonUtility::Vector3f & aInitialRotInDeg)
{
	myPosition.x = aPos.x;
	myPosition.y = aPos.y;
	myPosition.z = aPos.z;
	myPosition.w = 1.0f;
	Rotate(aInitialRotInDeg);
}

void Transform3D::Rotate(const float aRotationInDegrees, const CommonUtility::Vector3f & aRotationAxis)
{
	CommonUtility::Vector3f rotationAxis = aRotationAxis;
	rotationAxis.Normalize();
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationInDegrees), rotationAxis);
	UpdateTransform();
}

void Transform3D::Rotate(const float aRotationInDegrees, const CommonUtility::Vector4f & aRotationAxis)
{
	Rotate(aRotationInDegrees, {aRotationAxis.x, aRotationAxis.y, aRotationAxis.z});
}

void Transform3D::Rotate(const CommonUtility::Vector3f & aRotationVectorInDegrees)
{
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.x), CommonUtility::Vector3f(1, 0, 0));
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.y), CommonUtility::Vector3f(0, 1, 0));
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.z), CommonUtility::Vector3f(0, 0, 1));

	myOrientationMatrix *= CommonUtility::Matrix4x4f::CreateRotationAroundX(CommonUtility::DegToRadians(aRotationVectorInDegrees.x));
	myOrientationMatrix *= CommonUtility::Matrix4x4f::CreateRotationAroundY(CommonUtility::DegToRadians(aRotationVectorInDegrees.y));
	myOrientationMatrix *= CommonUtility::Matrix4x4f::CreateRotationAroundZ(CommonUtility::DegToRadians(aRotationVectorInDegrees.z));
	UpdateTransform();
}

void Transform3D::Rotate(const CommonUtility::Vector4f & aRotationVectorInDegrees)
{
	Rotate({ aRotationVectorInDegrees.x,aRotationVectorInDegrees.y,aRotationVectorInDegrees.z });
}

void Transform3D::RotateX(const float aRotationInDegrees)
{
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationInDegrees), CommonUtility::Vector3f(1, 0, 0));
	myOrientationMatrix *= CommonUtility::Matrix4x4f::CreateRotationAroundX(CommonUtility::DegToRadians(aRotationInDegrees));
	UpdateTransform();
}

void Transform3D::RotateY(const float aRotationInDegrees)
{
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationInDegrees), CommonUtility::Vector3f(0, 1, 0));
	myOrientationMatrix *= CommonUtility::Matrix4x4f::CreateRotationAroundY(CommonUtility::DegToRadians(aRotationInDegrees));
	UpdateTransform();
}

void Transform3D::RotateZ(const float aRotationInDegrees)
{
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationInDegrees), CommonUtility::Vector3f(0, 0, 1));
	//myOrientationMatrix *= CommonUtilities::Matrix4x4f::CreateRotationAroundZ(CommonUtilities::DegToRadians(aRotationInDegrees));
	UpdateTransform();
}

void Transform3D::Move(const CommonUtility::Vector3f & aDisplacementVector)
{
	MoveX(aDisplacementVector.x);
	MoveY(aDisplacementVector.y);
	MoveZ(aDisplacementVector.z);
}

void Transform3D::Move(const CommonUtility::Vector4f & aDisplacementVector)
{
	MoveX(aDisplacementVector.x);
	MoveY(aDisplacementVector.y);
	MoveZ(aDisplacementVector.z);
}

void Transform3D::MoveX(const float aDistance)
{
	myPosition += aDistance * CommonUtility::Vector4<float>({ myOrientationMatrix(1,1),myOrientationMatrix(1,2),myOrientationMatrix(1,3),myOrientationMatrix(1,4) });
	UpdateTransform();
}

void Transform3D::MoveY(const float aDistance)
{
	myPosition += aDistance * CommonUtility::Vector4<float>({ myOrientationMatrix(2,1),myOrientationMatrix(2,2),myOrientationMatrix(2,3),myOrientationMatrix(2,4) });
	UpdateTransform();
}

void Transform3D::MoveZ(const float aDistance)
{
	myPosition += aDistance * CommonUtility::Vector4<float>({ myOrientationMatrix(3,1),myOrientationMatrix(3,2),myOrientationMatrix(3,3),myOrientationMatrix(3,4) });
	UpdateTransform();
}

void Transform3D::MoveAbsolute(const float aDistance, const CU::Vector3f & aDirection)
{
	myPosition += aDistance * CU::Vector4f(aDirection.x, aDirection.y, aDirection.z, 0.0f);
	UpdateTransform();
}

void Transform3D::MoveAbsolute(const float aDistance, const CU::Vector4f & aDirection)
{
	myPosition += aDistance * CU::Vector4f(aDirection.x, aDirection.y, aDirection.z, 0.0f);
	UpdateTransform();
}

void Transform3D::MoveAbsolute(const CU::Vector3f & aDisplacementVec)
{
	myPosition += CU::ToVector4(aDisplacementVec, 0.0f);
	UpdateTransform();
}

void Transform3D::LookAt(const CommonUtility::Vector4f & aPointToLookAt)
{
	CommonUtility::Vector4f lookAtPoint = aPointToLookAt;
	lookAtPoint.w = 1;

	CU::Vector4f objectForward = GetRotationQuaternion().GetBasisVector3();
	CU::Vector4f desiredForward = lookAtPoint - GetPosition();
	desiredForward.Normalize();
	float cosTheta = acosf(objectForward.Dot(desiredForward));
	float angleDeg = CommonUtility::RadToDegrees(cosTheta);

	CU::Vector4f rotationAxis = desiredForward.Cross(objectForward);
	rotationAxis.Normalize();

	float exceptionThreshold = 1e-4f;

	if (rotationAxis.LengthSqr() < exceptionThreshold)
	{
		Rotate(angleDeg, GetRotationQuaternion().GetBasisVector2());
	}
	else
	{
		Rotate(angleDeg, rotationAxis);
	}
}


/*Another Way to look at a point, source from:
http://xboxforums.create.msdn.com/forums/t/10228.aspx */
void Transform3D::LookAtAlternate(const CommonUtility::Vector4f & aPointToLookAt)
{
	CommonUtility::Vector4f cardinalUp(0, 1, 0, 0);
	CommonUtility::Vector4f desiredForward = aPointToLookAt - GetPosition();
	desiredForward.Normalize();

	CommonUtility::Vector4f right;

	CommonUtility::Vector4f up;

	right = cardinalUp.Cross(desiredForward);

	//Extend function to take a transform from the start, then if right is a zero vector rotate around the right axis of the transform, basis vector 1.
	if (right.LengthSqr() == 0.0f)
	{
		CommonUtility::Vector4f objectForward = GetRotationQuaternion().GetBasisVector3();
		float cosTheta = objectForward.Dot(desiredForward.GetNormalized());
		float axisAngle = CommonUtility::RadToDegrees(acosf(cosTheta));
		int sign = static_cast<int>(desiredForward.y / fabsf(desiredForward.y));
		Rotate(axisAngle * sign, GetRotationQuaternion().GetBasisVector1());
		return;
	}
	right.Normalize();

	up = desiredForward.Cross(right);
	up.Normalize();

	CommonUtility::Matrix4x4f orientationAlt;
	orientationAlt.ConstructOrientation(right, up, desiredForward.GetNormalized());
	CommonUtility::Vector4f eulerAngles = orientationAlt.GetEulerAngles();
	SetEulerRotation(CommonUtility::RadToDegrees(eulerAngles.z), CommonUtility::RadToDegrees(eulerAngles.y), CommonUtility::RadToDegrees(eulerAngles.x));
	//assert(myOrientationMatrix == orientationAlt && "matrix does not match after rotation.");
	//CommonUtilities::Vector4f quat = orientationAlt.GetQuaternionRepresentation2();
	//CommonUtilities::Math3D::Quaternion orientQuat(quat.x, quat.y, quat.z, quat.w);
	//SetRotation(orientQuat);
	return;
}

void Transform3D::SetPosition(const CommonUtility::Vector3f & aPosition)
{
	myPosition.x = aPosition.x;
	myPosition.y = aPosition.y;
	myPosition.z = aPosition.z;
	myPosition.w = 1;
	UpdateTransform();
}

void Transform3D::SetPosition(const CommonUtility::Vector4f & aPosition)
{
	myPosition.x = aPosition.x;
	myPosition.y = aPosition.y;
	myPosition.z = aPosition.z;
	myPosition.w = 1;
	UpdateTransform();
}

void Transform3D::SetRotation(const CommonUtility::Vector3f & aRotationVectorInDegrees)
{
	myRotQuaternion = CommonUtility::Math3D::Quaternion();
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.x), CommonUtility::Vector3f(1, 0, 0));
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.y), CommonUtility::Vector3f(0, 1, 0));
	myRotQuaternion.Rotate(CommonUtility::DegToRadians(aRotationVectorInDegrees.z), CommonUtility::Vector3f(0, 0, 1));
	UpdateTransform();
}

void Transform3D::SetRotation(const CommonUtility::Vector4f & aRotationVectorInDegrees)
{
	SetRotation({aRotationVectorInDegrees.x, aRotationVectorInDegrees.y , aRotationVectorInDegrees.z });
}

void Transform3D::SetRotation(const float aRotationInDegrees, const CommonUtility::Vector3f & aRotationAxis)
{
	myRotQuaternion = CommonUtility::Math3D::Quaternion();
	Rotate(aRotationInDegrees, aRotationAxis);
}

void Transform3D::SetRotation(const CommonUtility::Math3D::Quaternion & aRotationQuaternion)
{
	myRotQuaternion = aRotationQuaternion;
	UpdateTransform();
}

void Transform3D::SetEulerRotation(const float aZInDegrees, const float aYInDegrees, const float aXInDegrees)
{
	SetRotation({0,0,0});
	RotateZ(aZInDegrees);
	RotateY(aYInDegrees);
	RotateX(aXInDegrees);
}

void Transform3D::SetEulerRotation(const CommonUtility::Vector4f & aRotationVectorInDegrees)
{
	SetEulerRotation(aRotationVectorInDegrees.z, aRotationVectorInDegrees.y, aRotationVectorInDegrees.x);
}

void Transform3D::SetEulerRotation(const CommonUtility::Vector3f & aRotationVectorInDegrees)
{
	SetEulerRotation(aRotationVectorInDegrees.z, aRotationVectorInDegrees.y, aRotationVectorInDegrees.x);
}

void Transform3D::UpdateTransform()
{
	myRotQuaternion.Normalize();
	myRotQuaternion.ConstructOrientation(myOrientationMatrix);
	myTransform = myOrientationMatrix;
	myTransform(4, 1) = myPosition.x;
	myTransform(4, 2) = myPosition.y;
	myTransform(4, 3) = myPosition.z;
	myTransform(4, 4) = 1;
	myPosition.w = 1.0f;
}

void Transform3D::SetTransform(const CommonUtility::Matrix4x4f& aTransform)
{
	myTransform = aTransform;
}