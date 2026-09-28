#include "ViewportNavigation.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	void ViewportNavigation::Update(float deltaTime, Viewport& viewport, EditorCamera& camera)
	{
		if (Input::GetKey(Key::LeftAlt) && Input::GetKeyDown(Key::Enter))
			viewport.SetFocus(true);

		if (!viewport.IsFocused())
			return;

		// WASDQE movement
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

		// Look around
		if (Input::GetKey(Key::Up)) mCameraPitch += mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Down)) mCameraPitch -= mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Left)) mCameraYaw += mCameraLookSpeed * deltaTime;
		if (Input::GetKey(Key::Right)) mCameraYaw -= mCameraLookSpeed * deltaTime;

		mCameraPitch = glm::clamp(mCameraPitch, glm::radians(- 89.9f), glm::radians(89.9f));
		camera.transform.rotation = glm::angleAxis(mCameraYaw, glm::vec3(0, 1, 0));
		camera.transform.rotation = camera.transform.rotation * glm::angleAxis(mCameraPitch, glm::vec3(1, 0, 0));
	}
}