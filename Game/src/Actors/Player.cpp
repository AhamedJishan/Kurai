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
		mCamera->GetTransform().Position = GetTransform().Position + mCameraOffset;

		float yaw = mCamera->GetYaw();
		GetTransform().Rotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));

		// WASD movement
		glm::vec3 moveDir = glm::vec3(0);

		if (Input::GetKey(Key::W)) moveDir += GetTransform().GetForward();
		if (Input::GetKey(Key::S)) moveDir -= GetTransform().GetForward();
		if (Input::GetKey(Key::A)) moveDir -= GetTransform().GetRight();
		if (Input::GetKey(Key::D)) moveDir += GetTransform().GetRight();

		if (glm::length(moveDir) > 0.0001f)
			moveDir = glm::normalize(moveDir);

		GetTransform().Position += moveDir * mSpeed * deltaTime;
	}

	glm::vec3 Player::GetGunPosition()
	{
		glm::vec3 gunPos = mCamera->GetTransform().Position;

		gunPos += mCamera->GetTransform().GetUp() * mGunOffset.y;
		gunPos += mCamera->GetTransform().GetRight() * mGunOffset.x;
		gunPos += mCamera->GetTransform().GetForward() * mGunOffset.z;

		return gunPos;
	}
}