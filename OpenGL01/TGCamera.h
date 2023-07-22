#pragma once
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/ext/matrix_clip_space.hpp"

enum class ETGCameraProjectionType : unsigned char
{
	TGCameraProjection_Perspective = 0,
	TGCameraProjection_Orthogonal = 1,
};

class TGCameraViewInfo
{
public:
	// Parameter For 
	float mFov = 90.0;
	float mAspectRatio = 1.0;

	float mLeft = -1.0;
	float mRight = 1.0;
	float mTop = 1.0;
	float mBottom = -1.0;
};

class TGCamera
{
	TGCamera();

	void SetCameraLookAt(glm::vec3 Position, glm::vec3 Target);

private:
	glm::vec3 mCameraPosition;
	glm::vec3 mCameraTarget;
	glm::vec3 mCameraDirection;
	glm::vec3 mCameraRight;
	glm::vec3 mCameraUp;

	glm::mat4x4 mViewMatrix;

	ETGCameraProjectionType mCameraProjectionType = ETGCameraProjectionType::TGCameraProjection_Perspective;
};