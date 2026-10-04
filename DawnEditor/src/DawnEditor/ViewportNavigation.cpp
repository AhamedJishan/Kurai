#include "ViewportNavigation.h"

#include <glm/gtc/quaternion.hpp>
#include <Dawn/Input/Input.h>
#include <Dawn/Core/Actor.h>

namespace Dawn
{
	void ViewportNavigation::Update(float deltaTime, Viewport& viewport, EditorCamera& camera, Actor* selectedActor)
	{
		if (Input::GetKey(Key::LeftAlt) && Input::GetKeyDown(Key::Enter))
			viewport.SetFocus(true);

		if (!viewport.IsFocused())
			return;

		// --- WASDQE movement ---
		glm::vec3 moveDir = { 0, 0, 0 };
		if (Input::GetKey(Key::W)) moveDir += camera.transform.GetForward();
		if (Input::GetKey(Key::S)) moveDir -= camera.transform.GetForward();
		if (Input::GetKey(Key::A)) moveDir -= camera.transform.GetRight();
		if (Input::GetKey(Key::D)) moveDir += camera.transform.GetRight();
		if (Input::GetKey(Key::Q)) moveDir -= camera.transform.GetUp();
		if (Input::GetKey(Key::E)) moveDir += camera.transform.GetUp();
		if (glm::length(moveDir) > 0)
			moveDir = glm::normalize(moveDir);
		if (Input::GetKey(Key::LeftShift))
			moveDir *= mCameraSlowMoveMultiplier;

		camera.transform.position += moveDir * mCameraMoveSpeed * deltaTime;

		// --- Look around ---
		if (Input::GetKey(Key::Up)) mCameraPitch += mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Down)) mCameraPitch -= mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Left)) mCameraYaw += mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Right)) mCameraYaw -= mCameraLookSpeed * deltaTime;

		mCameraPitch = glm::clamp(mCameraPitch, glm::radians(- 89.9f), glm::radians(89.9f));
		camera.transform.rotation = glm::angleAxis(mCameraYaw, glm::vec3(0, 1, 0));
		camera.transform.rotation = camera.transform.rotation * glm::angleAxis(mCameraPitch, glm::vec3(1, 0, 0));

		// --- Focus selected ---
		if (selectedActor && Input::GetKeyDown(Key::F))
		{
			mShouldFocusActor = true;
			mActorFocusPosition = selectedActor->GetTransform().position - camera.transform.GetForward() * mActorFocusDistance;
		}

		if(mShouldFocusActor)
		{
			if (glm::length(mActorFocusPosition - camera.transform.position) > 0.1f)
			{
				float t = 1.0f - glm::exp(-mActorFocusSpeed * deltaTime);
				camera.transform.position = glm::mix(camera.transform.position, mActorFocusPosition, t);
			}

			if (glm::length(mActorFocusPosition - camera.transform.position) <= 0.1f)
				mShouldFocusActor = false;
		}
	}
}