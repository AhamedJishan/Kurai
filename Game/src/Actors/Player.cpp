#include "Player.h"
#include "Utils/Log.h"

#include "FPSCameraActor.h"
#include "Core/Scene.h"

namespace Dawn
{
	Player::Player(Scene* scene, FPSCameraActor* fpsCameraActor)
		:Actor(scene)
	{
		if (!fpsCameraActor)
		{
			LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
			return;
		}

		mCamera = fpsCameraActor;
	}

	void Player::Update(float deltaTime)
	{
		// sync camera pos
		mCamera->SetPosition(GetPosition() + mCameraOffset);

		float yaw = mCamera->GetYaw();
		SetRotation(glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)));

		// WASD movement
		glm::vec3 moveDir = glm::vec3(0);

		if (Input::GetKey(Key::W)) moveDir += GetForward();
		if (Input::GetKey(Key::S)) moveDir -= GetForward();
		if (Input::GetKey(Key::A)) moveDir -= GetRight();
		if (Input::GetKey(Key::D)) moveDir += GetRight();

		if (glm::length(moveDir) > 0.0001f)
			moveDir = glm::normalize(moveDir);

		SetPosition(GetPosition() + moveDir * mSpeed * deltaTime);
	}

	glm::vec3 Player::GetGunPosition()
	{
		glm::vec3 gunPos = mCamera->GetPosition();

		gunPos += mCamera->GetUp() * mGunOffset.y;
		gunPos += mCamera->GetRight() * mGunOffset.x;
		gunPos += mCamera->GetForward() * mGunOffset.z;

		return gunPos;
	}
}