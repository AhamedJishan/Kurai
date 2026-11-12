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

	static const float REFERENCE_ASPECT_RATIO = 16.0f / 9.0f;

	glm::mat4 EditorCamera::GetProjection(float aspectRatio) const
	{
		float t = glm::tan(glm::radians(fov) * 0.5f);		// Half height at unit distance

		if (aspectRatio > REFERENCE_ASPECT_RATIO)			// new t = adjusted_width_at_ref_aspect_ratio / aspectRatio  
			t *= REFERENCE_ASPECT_RATIO / aspectRatio;		// where adjusted_width_at_ref_aspect_ratio = t * ref_aspect_ratio		
															
		return glm::perspective(2.0f * glm::atan(t), aspectRatio, near, far);
	}
}