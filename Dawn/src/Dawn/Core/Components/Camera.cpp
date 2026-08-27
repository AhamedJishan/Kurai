#include "Camera.h"

#include "Dawn/Core/Actor.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Window.h>
#include <Dawn/Audio/AudioSystem.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Core/Property.h>

namespace Dawn
{
	std::vector<Property> Camera::GetProperties()
	{
		return 
		{
			{"FOV", &mFOV, PropertyType::Float},
			{"Near", &mNear, PropertyType::Float},
			{"Far", &mFar, PropertyType::Float}
		};
	}

	Camera::Camera(Actor* owner)
		:Component(owner)
	{
	}

	void Camera::Update(float deltaTime)
	{
		if (mFOV <= 0.0f)
			mFOV = 0.1f;

		Application::Get()->GetAudioSystem()->SetListener(GetView());
	}

	glm::mat4 Camera::GetView() const
	{
		glm::vec3 position = mOwner->GetTransform().Position;
		glm::vec3 forward  = mOwner->GetTransform().GetForward();
		glm::vec3 up = mOwner->GetTransform().GetUp();

		return glm::lookAt(position, position + forward, up);
	}

	glm::vec3 Camera::UnProject(float screenX, float screenY, float depth)
	{
		float screenWidth = static_cast<float>(Application::Get()->GetWindow()->GetWidth());
		float screenHeight = static_cast<float>(Application::Get()->GetWindow()->GetHeight());
		glm::vec2 renderResolution = Application::Get()->GetRenderer()->GetResolution();

		float ndcX = (screenX / screenWidth) * 2.0f - 1.0f;
		float ndcY = 1.0f - (screenY / screenHeight) * 2.0f;
		float ndcZ = depth * 2.0f - 1.0f;
		glm::vec4 clipPos(ndcX, ndcY, ndcZ, 1.0f);

		glm::mat4 projection = glm::perspectiveFov(glm::radians(mFOV), renderResolution.x, renderResolution.y, mNear, mFar);
		glm::mat4 invViewProjection = glm::inverse(projection * GetView());

		glm::vec4 worldPos = invViewProjection * clipPos;
		worldPos /= worldPos.w;

		return glm::vec3(worldPos);
	}
}