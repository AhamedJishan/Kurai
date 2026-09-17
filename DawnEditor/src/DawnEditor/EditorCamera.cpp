#include "EditorCamera.h"

#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>

namespace Dawn
{
	void EditorCamera::Update(float deltaTime)
	{
		Application::Get()->GetRenderer()->SetView(GetView());
		Application::Get()->GetRenderer()->SetProjection(GetProjection());
	}

	glm::mat4 EditorCamera::GetView() const
	{
		return glm::lookAt(transform.Position, transform.Position + transform.GetForward(), transform.GetUp());
	}

	glm::mat4 EditorCamera::GetProjection() const
	{
		glm::vec2 resolution = Application::Get()->GetRenderer()->GetResolution();
		return glm::perspectiveFov(glm::radians(fov), resolution.x, resolution.y, near, far);
	}
}