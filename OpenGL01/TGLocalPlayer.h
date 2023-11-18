#pragma once
#include "TGCamera.h"
#include <string>

class TGLocalPlayer
{
public:
	TGLocalPlayer(std::string PlayerID);

	TGCamera* GetPlayerCamera();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void ViewUp(float Value);

	void ViewRight(float Value);

private:
	TGCamera* mPlayerCamera = nullptr;

	std::string mPlayerID;

	glm::vec3 mPlayerLocation;

	// Roll Yaw Pitch
	float mRow = 0.0;

	float mYaw = 0.0;

	float mPitch = 0.0;

	float mMoveSpeed = 1.0;
};