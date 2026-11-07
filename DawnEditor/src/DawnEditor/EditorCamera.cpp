#include "EditorCamera.h"

#include <Dawn/Core/Application.h>
#include <Dawn/Audio/AudioSystem.h>

namespace Dawn
{
	void EditorCamera::Update()
	{
		Application::Get()->GetAudioSystem()->SetListener(GetView());
	}

	glm::mat4 EditorCamera::GetView() const
	{
		return glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	}

	glm::mat4 EditorCamera::GetProjection(float aspectRatio) const
	{
		return glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}
}