#include "EditorCamera.h"

#include <glm/vec2.hpp>
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Audio/AudioSystem.h>

namespace Dawn
{
	void EditorCamera::Update()
	{
		Application::Get()->GetRenderer()->SetView(GetView());
		Application::Get()->GetRenderer()->SetProjection(GetProjection());
		Application::Get()->GetAudioSystem()->SetListener(GetView());
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