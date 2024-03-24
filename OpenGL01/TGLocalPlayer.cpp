#include "TGLocalPlayer.h"
#include "TGCamera.h"
#include <iostream>

TGLocalPlayer::TGLocalPlayer(std::string PlayerID)
{
	mPlayerID = PlayerID;

	mPlayerCamera = new TGCamera();

	mPlayerCamera->SetCameraPosition(mPlayerLocation);
}

TGCamera* TGLocalPlayer::GetPlayerCamera()
{
	return mPlayerCamera;
}

void TGLocalPlayer::MoveForward(float Value)
{
	if (mPlayerCamera)
	{
		glm::vec3 forwardDirection = mPlayerCamera->GetForwardAxis();

		mPlayerLocation += forwardDirection * mMoveSpeed * Value;

		mPlayerCamera->SetCameraPosition(mPlayerLocation);
	}
}

void TGLocalPlayer::MoveRight(float Value)
{
	if (mPlayerCamera)
	{
		glm::vec3 rightDirection = mPlayerCamera->GetRightAxis();

		mPlayerLocation += rightDirection * mMoveSpeed * Value;

		mPlayerCamera->SetCameraPosition(mPlayerLocation);
	}
}

void TGLocalPlayer::MoveUp(float Value)
{
	if (mPlayerCamera)
	{
		glm::vec3 upDirection = mPlayerCamera->GetUpAxis();

		mPlayerLocation += upDirection * mMoveSpeed * Value;

		mPlayerCamera->SetCameraPosition(mPlayerLocation);
	}
}

void TGLocalPlayer::ViewUp(float Value)
{
	mPitch += Value;
	if (mPitch > 89.9) mPitch = 89.9;
	if (mPitch < -89.9) mPitch = -89.9;

	if (mPlayerCamera)
	{
		// std::cout << "Input RPY: " << mRow << "," << mPitch << "," << mYaw << std::endl;
		mPlayerCamera->SetCameraRotation(mRoll, mPitch, mYaw);
	}
}

void TGLocalPlayer::ViewRight(float Value)
{
	mYaw += Value;
	if (mYaw >= 360.0) mYaw -= 360.0;
	if (mYaw < 0) mYaw += 360.0;

	if (mPlayerCamera)
	{
		// std::cout << "Input RPY: " << mRow << "," << mPitch << "," << mYaw << std::endl;
		mPlayerCamera->SetCameraRotation(mRoll, mPitch, mYaw);
	}
}

void TGLocalPlayer::SetPlayerPosition(glm::vec3 position)
{
	mPlayerLocation = position;

	mPlayerCamera->SetCameraPosition(position);
}

void TGLocalPlayer::SetPlayerLookAt(glm::vec3(position))
{
	mPlayerCamera->SetCameraLookAt(mPlayerLocation, position);

	mPitch = mPlayerCamera->mPitch;
	mYaw = mPlayerCamera->mYaw;
	mRoll = mPlayerCamera->mRoll;
}

