#pragma once
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Quaternion.h"

class Transform3D
{
public:
	Transform3D();
	~Transform3D();

	void Init(const CommonUtility::Vector3f& aPos, const CommonUtility::Vector3f& aInitialRotInDeg);

	void Rotate(const float aRotationInDegrees, const CommonUtility::Vector3f& aRotationAxis);
	void Rotate(const float aRotationInDegrees, const CommonUtility::Vector4f& aRotationAxis);
	void Rotate(const CommonUtility::Vector3f& aRotationVectorInDegrees);
	void Rotate(const CommonUtility::Vector4f& aRotationVectorInDegrees);
	void RotateX(const float aRotationInDegrees);
	void RotateY(const float aRotationInDegrees);
	void RotateZ(const float aRotationInDegrees);

	void Move(const CommonUtility::Vector3f& aDisplacementVector);
	void Move(const CommonUtility::Vector4f& aDisplacementVector);
	void MoveX(const float aDistance);
	void MoveY(const float aDistance);
	void MoveZ(const float aDistance);

	void MoveAbsolute(const CU::Vector3f& aDisplacementVec);
	void MoveAbsolute(const float aDistance, const CU::Vector3f& aDirection);
	void MoveAbsolute(const float aDistance, const CU::Vector4f& aDirection);

	void LookAt(const CommonUtility::Vector4f& aPointToLookAt);
	void LookAtAlternate(const CommonUtility::Vector4f & aPointToLookAt);

	void SetPosition(const CommonUtility::Vector3f& aPosition);
	void SetPosition(const CommonUtility::Vector4f& aPosition);
	void SetRotation(const CommonUtility::Vector3f& aRotationVectorInDegrees);
	void SetRotation(const CommonUtility::Vector4f& aRotationVectorInDegrees);
	void SetRotation(const float aRotationInDegrees, const CommonUtility::Vector3f& aRotationAxis);
	void SetRotation(const CommonUtility::Math3D::Quaternion& aRotationQuaternion);

	void SetTransform(const CommonUtility::Matrix4x4f& aTransform);
	
	void SetEulerRotation(const float aZInDegrees, const float aYInDegrees, const float aXInDegrees);
	void SetEulerRotation(const CommonUtility::Vector4f& aRotationVectorInDegrees);
	void SetEulerRotation(const CommonUtility::Vector3f& aRotationVectorInDegrees);

	const CommonUtility::Matrix4x4f& GetTransform() const { return myTransform; }
	const CommonUtility::Vector4f& GetPosition() const { return myPosition; }
	const CommonUtility::Matrix4x4f& GetOrientation() const { return myOrientationMatrix; }
	const CommonUtility::Matrix4x4f GetOrientationInverse() const { return CommonUtility::Matrix4x4f::Transpose(myOrientationMatrix); }
	const CommonUtility::Math3D::Quaternion& GetRotationQuaternion() const { return myRotQuaternion; }
private:

	void UpdateTransform();

	CommonUtility::Matrix4x4f myOrientationMatrix;
	CommonUtility::Matrix4x4f myTransform;
	CommonUtility::Math3D::Quaternion myRotQuaternion;
	CommonUtility::Vector4f myPosition;
};

