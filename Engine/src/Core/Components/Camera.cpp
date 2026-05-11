#include "Camera.h"

#include "Core/Actor.h"
#include <Core/Application.h>
#include <Core/Window.h>
#include <Audio/AudioSystem.h>

namespace Dawn
{
	Camera::Camera(Actor* owner, unsigned int updateOrder)
		:Component(owner, updateOrder)
	{
	}

	void Camera::Update(float deltaTime)
	{
		Application::Get()->GetAudioSystem()->SetListener(GetView());
	}

	glm::mat4 Camera::GetView() const
	{
		glm::vec3 position = mOwner->GetPosition();
		glm::vec3 forward  = mOwner->GetForward();
		glm::vec3 up = mOwner->GetUp();

		return glm::lookAt(position, position + forward, up);
	}
	
	glm::mat4 Camera::GetProjection() const
	{
		int width = Application::Get()->GetWindow()->GetWidth();
		int height = Application::Get()->GetWindow()->GetHeight();

		if (height == 0) height = 1;
		float aspect = static_cast<float>(width) / static_cast<float>(height);

		return glm::perspective(glm::radians(mFOV), aspect, mNear, mFar);
	}

	glm::vec3 Camera::UnProject(float screenX, float screenY, float depth)
	{
		float screenWidth = static_cast<float>(Application::Get()->GetWindow()->GetWidth());
		float screenHeight = static_cast<float>(Application::Get()->GetWindow()->GetHeight());

		float ndcX = (screenX / screenWidth) * 2.0f - 1.0f;
		float ndcY = 1.0f - (screenY / screenHeight) * 2.0f;
		float ndcZ = depth * 2.0f - 1.0f;
		glm::vec4 clipPos(ndcX, ndcY, ndcZ, 1.0f);

		glm::mat4 invViewProjection = glm::inverse(GetProjection() * GetView());

		glm::vec4 worldPos = invViewProjection * clipPos;
		worldPos /= worldPos.w;

		return glm::vec3(worldPos);
	}
}