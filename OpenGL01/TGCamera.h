#pragma once
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/ext/matrix_clip_space.hpp"

class TGLocalPlayer;

enum class ETGCameraProjectionType : unsigned char
{
	TGCameraProjection_Perspective = 0,
	TGCameraProjection_Orthogonal = 1,
};

class TGCameraViewInfo
{
public:
	// Parameter For Perspective
	float mFov = 90.0f;
	float mAspectRatio = 1.0f;
	float mNear = 0.1f;
	float mFar = 100.0f;

	// Parameter For Orthogonal
	float mLeft = -1.0f;
	float mRight = 1.0f;
	float mTop = 1.0f;
	float mBottom = -1.0f;
};

class TGCamera
{
public:
	friend class TGLocalPlayer;

public:
	TGCamera();

	void SetCameraLookAt(const glm::vec3 Position, const glm::vec3 Target);

	void SetCameraViewInfo(const TGCameraViewInfo CameraViewInfo);

	void SetCameraPosition(const glm::vec3 Position);

	void SetCameraRotation(const glm::vec3 Rotation);

	void SetCameraRotation(const float Roll, const float Pitch, const float Yaw);

	void SetCameraProjectionType(const ETGCameraProjectionType Type);

	glm::mat4x4 GetCameraViewMatrix();

	glm::mat4x4 GetCameraProjectionMatrix();

	glm::vec3 GetForwardAxis();

	glm::vec3 GetRightAxis();

	glm::vec3 GetUpAxis();

	float GetCameraRoll();

	float GetCameraYaw();

	float GetCameraPitch();

	glm::vec3 GetCameraPosition();

	glm::vec3 GetCameraRotation();

private:
	void UpdateInsideEulerAngle();

	void UpdateInsideMatrix();

private:
	glm::vec3 mCameraPosition;
	glm::vec3 mCameraTarget;
	glm::vec3 mCameraDirection;
	glm::vec3 mCameraRight;
	glm::vec3 mCameraUp;

	glm::vec3 mWorldUp;

	float mRoll = 0.0;
	float mYaw = 0.0;
	float mPitch = 0.0;

	glm::mat4x4 mViewMatrix;
	glm::mat4x4 mProjectionMatrix;

	ETGCameraProjectionType mCameraProjectionType = ETGCameraProjectionType::TGCameraProjection_Perspective;
	TGCameraViewInfo mCameraViewInfo;
};