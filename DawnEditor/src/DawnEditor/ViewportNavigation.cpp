#include "ViewportNavigation.h"

#include <glm/vec3.hpp>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	void ViewportNavigation::Update(float deltaTime, Viewport& viewport, EditorCamera& camera)
	{
		if (Input::GetKey(Key::LeftAlt) && Input::GetKeyDown(Key::Enter))
			viewport.SetFocus(true);

		if (!viewport.IsFocused())
			return;

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
	}
}