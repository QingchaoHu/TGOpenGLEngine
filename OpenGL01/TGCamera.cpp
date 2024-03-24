#include "TGCamera.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/ext/matrix_transform.hpp"
#include <iostream>

TGCamera::TGCamera()
{
	mCameraPosition = glm::vec3(1.0f, 0.0f, 1.0f);
	mCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	mWorldUp = glm::vec3(0.0f, 0.0f, 1.0f);

	SetCameraLookAt(mCameraPosition, mCameraTarget);
}

void TGCamera::SetCameraLookAt(glm::vec3 Position, glm::vec3 Target)
{
	mCameraPosition = Position;
	mCameraTarget = Target;

	mCameraDirection = glm::normalize(mCameraPosition - mCameraTarget);
	mCameraRight = glm::normalize(glm::cross(mCameraDirection, mWorldUp));
	mCameraUp = glm::normalize(glm::cross(mCameraDirection, mCameraRight));

	// std::cout << "TGCamera Location:" << mCameraPosition.x << "," << mCameraPosition.y << "," << mCameraPosition.z << std::endl;
	// std::cout << "TGCamera Forward:" << mCameraDirection.x << "," << mCameraDirection.y << "," << mCameraDirection.z << std::endl;
	// std::cout << "TGCamera RPY:" << mRoll << "," << mPitch << "," << mYaw << std::endl;

	UpdateInsideEulerAngle();

	UpdateInsideMatrix();
}

void TGCamera::SetCameraViewInfo(const TGCameraViewInfo CameraViewInfo)
{
	mCameraViewInfo = CameraViewInfo;

	UpdateInsideMatrix();
}

TGCameraViewInfo TGCamera::getCameraViewInfo() {
	return mCameraViewInfo;
}

void TGCamera::SetCameraPosition(const glm::vec3 Position)
{
	// 重新设置相机位置，内部的Direction是反的，所以要反算
	mCameraPosition = Position;

	mCameraTarget = mCameraPosition - mCameraDirection;

	SetCameraLookAt(mCameraPosition, mCameraTarget);
}

void TGCamera::SetCameraRotation(const glm::vec3 Rotation)
{
	// 传入的Direction是正的，因此需要正算
	mCameraTarget = mCameraPosition + Rotation;

	SetCameraLookAt(mCameraPosition, mCameraTarget);
}

void TGCamera::SetCameraRotation(const float Roll, const float Pitch, const float Yaw)
{
	mRoll = Roll;
	mPitch = Pitch;
	mYaw = Yaw;
	
	float directionX = glm::cos(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	float directionY = glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	float directionZ = glm::sin(glm::radians(mPitch));

	glm::vec3 direction(directionX, directionY, directionZ);
	direction = glm::normalize(direction);

	SetCameraRotation(direction);
}

void TGCamera::SetCameraProjectionType(const ETGCameraProjectionType Type)
{
	mCameraProjectionType = Type;

	UpdateInsideMatrix();
}

glm::mat4x4 TGCamera::GetCameraViewMatrix()
{
	return mViewMatrix;
}

glm::mat4x4 TGCamera::GetCameraProjectionMatrix()
{
	return mProjectionMatrix;
}

glm::vec3 TGCamera::GetForwardAxis()
{
	return -mCameraDirection;
}

glm::vec3 TGCamera::GetRightAxis()
{
	return mCameraRight;
}

glm::vec3 TGCamera::GetUpAxis()
{
	return mCameraUp;
}

float TGCamera::GetCameraRoll()
{
	return mRoll;
}

float TGCamera::GetCameraYaw()
{
	return mYaw;
}

float TGCamera::GetCameraPitch()
{
	return mPitch;
}

glm::vec3 TGCamera::GetCameraPosition()
{
	return mCameraPosition;
}

glm::vec3 TGCamera::GetCameraRotation()
{
	return mCameraDirection;
}

void TGCamera::UpdateInsideEulerAngle()
{
	glm::vec3 direction = -mCameraDirection;
	glm::vec3 up = mCameraUp;

	glm::vec3 heading(direction.x, direction.y, 0);
	float angleH = atan2(heading.y, heading.x);

	float angleP = glm::asin(direction.z);

	glm::vec3 W0(-direction.y, direction.x, 0);
	glm::vec3 U0 = W0 * direction;
	float angleB = atan2(glm::dot(W0, up) / glm::length(W0), glm::dot(U0, up) / glm::length(U0));

	mYaw = glm::degrees(angleH);
	mPitch = glm::degrees(angleP);
	mRoll = glm::degrees(angleB);
}

void TGCamera::UpdateInsideMatrix()
{
	glm::mat4x4 rotateMatrix(
		glm::vec4(mCameraRight, 0),
		glm::vec4(mCameraUp, 0),
		glm::vec4(mCameraDirection, 0),
		glm::vec4(0, 0, 0, 1));

	glm::mat4x4 translateMatrix(1.0);
	translateMatrix = glm::translate(translateMatrix, mCameraPosition);

	mViewMatrix = rotateMatrix * translateMatrix;

	glm::mat4x4 lookAtMatrix = glm::lookAt(mCameraPosition, mCameraTarget, mWorldUp);

	// lookAtMatrix = glm::rotate(lookAtMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	mViewMatrix = lookAtMatrix;

	if (mCameraProjectionType == ETGCameraProjectionType::TGCameraProjection_Perspective)
	{
		mProjectionMatrix = glm::perspective(glm::radians((float)mCameraViewInfo.mFov), mCameraViewInfo.mAspectRatio, mCameraViewInfo.mNear, mCameraViewInfo.mFar);
	}
	else if (mCameraProjectionType == ETGCameraProjectionType::TGCameraProjection_Orthogonal)
	{
		mProjectionMatrix = glm::ortho(mCameraViewInfo.mLeft, mCameraViewInfo.mRight, mCameraViewInfo.mBottom, mCameraViewInfo.mTop);
	}
	else
	{
		mProjectionMatrix = glm::mat4x4(1.0);
	}
}
