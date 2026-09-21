#include "EditorCamera.h"

#include <glm/vec2.hpp>
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	void EditorCamera::Update(float deltaTime, bool receivesInput)
	{
		Application::Get()->GetRenderer()->SetView(GetView());
		Application::Get()->GetRenderer()->SetProjection(GetProjection());

		if (receivesInput)
			HandleNavigation(deltaTime);
	}

	glm::mat4 EditorCamera::GetView() const
	{
		return glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	}

	glm::mat4 EditorCamera::GetProjection() const
	{
		glm::vec2 resolution = Application::Get()->GetRenderer()->GetResolution();
		return glm::perspectiveFov(glm::radians(fov), resolution.x, resolution.y, near, far);
	}

	void EditorCamera::HandleNavigation(float deltaTime)
	{
		if (Input::GetMouseButton(MouseButton::Right))
		{
			// Look around
			glm::vec2 mouseDeltaPos = Input::GetCursorDeltaPos();

			mYaw -= mouseDeltaPos.x * mLookSensitivity;
			mPitch -= mouseDeltaPos.y * mLookSensitivity;
			mPitch = glm::clamp(mPitch, glm::radians(-89.0f), glm::radians(89.0f));

			transform.rotation = glm::angleAxis(mYaw, glm::vec3(0, 1, 0));
			transform.rotation = transform.rotation * glm::angleAxis(mPitch, glm::vec3(1, 0, 0));

			// WASD movement
			glm::vec3 moveDir = { 0, 0, 0 };

			if (Input::GetKey(Key::W)) moveDir += transform.GetForward();
			if (Input::GetKey(Key::S)) moveDir -= transform.GetForward();
			if (Input::GetKey(Key::A)) moveDir -= transform.GetRight();
			if (Input::GetKey(Key::D)) moveDir += transform.GetRight();
			if (Input::GetKey(Key::Q)) moveDir -= transform.GetUp();
			if (Input::GetKey(Key::E)) moveDir += transform.GetUp();

			if (glm::length(moveDir) > 0.0f)
				moveDir = glm::normalize(moveDir);

			if (Input::GetKey(Key::LeftShift))
				moveDir *= mSlowMoveMultiplier;

			transform.position += moveDir * mMoveSpeed * deltaTime;
		}
	}
}