#pragma once

#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Core/Actor.h"
#include "Core/Components/Camera.h"
#include "Input/Input.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class FPSCamera : public Camera
	{
	public:
		FPSCamera(Actor* owner, unsigned int updateOrder = 100)
			:Camera(owner, updateOrder)
		{
		}

		void Update(float deltaTime) override
		{
			Camera::Update(deltaTime);

			glm::vec2 cursorDeltaPos = Input::GetCursorDeltaPos();

			mYaw += -cursorDeltaPos.x * yawSpeed;
			mPitch += -cursorDeltaPos.y * pitchSpeed;

			mPitch = glm::clamp(mPitch, -60.0f, 60.0f);

			// Recoil
			mRecoilPitchOffset = glm::mix(mRecoilPitchOffset, 0.0f, mRecoilRecoverySpeed * deltaTime);
			float finalPitch = glm::clamp(mPitch + mRecoilPitchOffset, -60.0f, 60.0f);

			glm::quat yawRotation	= glm::angleAxis(glm::radians(mYaw),		glm::vec3(0, 1, 0));
			glm::quat pitchRotation = glm::angleAxis(glm::radians(finalPitch),	glm::vec3(1, 0, 0));

			mOwner->SetRotation(yawRotation * pitchRotation);
		}

		void Recoil()
		{
			mRecoilPitchOffset = mRecoilPitchAmount;
		}

		float GetYaw() const { return mYaw; }
		float GetPitch() const { return mPitch; }

	private:
		// In degrees
		float yawSpeed = 0.1f;
		float pitchSpeed = 0.1f;

		float mYaw = 0.0f;
		float mPitch = 0.0f;

		// Recoil
		const float mRecoilRecoverySpeed = 5.0f;
		const float mRecoilPitchAmount = 1.5f;
		float mRecoilPitchOffset = 0.0f;
	};
}