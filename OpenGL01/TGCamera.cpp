#include "TGCamera.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/ext/matrix_transform.hpp"

TGCamera::TGCamera()
{
	mCameraPosition = glm::vec3(10.0, 10.0, 10.0);
	mCameraTarget = glm::vec3(0.0, 0.0, 0.0);

	SetCameraLookAt(mCameraPosition, mCameraTarget);
}

void TGCamera::SetCameraLookAt(glm::vec3 Position, glm::vec3 Target)
{
	mCameraPosition = Position;
	mCameraTarget = Target;

	mCameraDirection = glm::normalize(mCameraPosition - mCameraTarget);
	mCameraRight = glm::normalize(glm::cross(mCameraDirection, glm::vec3(0, 1, 0)));
	mCameraUp = glm::normalize(glm::cross(mCameraDirection, mCameraRight));

	glm::mat4x4 rotateMatrix(
		glm::vec4(mCameraRight, 0), 
		glm::vec4(mCameraUp, 0), 
		glm::vec4(mCameraDirection, 0), 
		glm::vec4(0, 0, 0, 1));

	glm::mat4x4 translateMatrix(1.0);
	translateMatrix = glm::translate(translateMatrix, mCameraPosition);

	mViewMatrix = rotateMatrix * translateMatrix;

	glm::mat4x4 lookAtMatrix = glm::lookAt(mCameraPosition, mCameraTarget, glm::vec3(0, 1, 0));

	mViewMatrix = lookAtMatrix;
}