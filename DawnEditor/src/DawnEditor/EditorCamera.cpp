#include "EditorCamera.h"

#include <glm/vec2.hpp>
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	void EditorCamera::Update(float deltaTime)
	{
		Application::Get()->GetRenderer()->SetView(GetView());
		Application::Get()->GetRenderer()->SetProjection(GetProjection());

		if (Input::GetMouseButton(MouseButton::Right))
		{
			glm::vec2 mouseDeltaPos = Input::GetCursorDeltaPos();

			mYaw -= mouseDeltaPos.x * mLookSensitivity;
			mPitch -= mouseDeltaPos.y * mLookSensitivity;
			mPitch = glm::clamp(mPitch, glm::radians(-89.0f), glm::radians(89.0f));

			transform.rotation = glm::angleAxis(mYaw, glm::vec3(0, 1, 0));
			transform.rotation = transform.rotation * glm::angleAxis(mPitch, glm::vec3(1, 0, 0));
		}
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
}