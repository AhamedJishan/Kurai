#include "Camera.h"

#include "Dawn/Core/Actor.h"
#include <Dawn/Core/Property.h>

namespace Dawn
{
	static const float MIN_FOV = 1.0f;
	static const float MAX_FOV = 170.0f;
	static const float MIN_NEAR = 0.01f;
	static const float MIN_PLANE_GAP = 0.001f;
	static const glm::uvec2 MIN_RESOLUTION = glm::uvec2(2);

	std::vector<Property> Camera::GetProperties()
	{
		return 
		{
			{"FOV", &mFOV, PropertyType::Float},
			{"Near", &mNear, PropertyType::Float},
			{"Far", &mFar, PropertyType::Float},
			{"Resolution", &mResolution, PropertyType::UVec2}
		};
	}

	Camera::Camera(Actor* owner)
		:Component(owner)
	{
	}

	void Camera::OnPropertiesChanged()
	{
		SetFOV(mFOV);
		SetNearPlane(mNear);
		SetFarPlane(mFar);
		SetResolution(mResolution);
	}

	glm::mat4 Camera::GetView() const
	{
		glm::vec3 position = mOwner->GetTransform().position;
		glm::vec3 forward  = mOwner->GetTransform().GetForward();
		glm::vec3 up = mOwner->GetTransform().GetUp();

		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 Camera::GetProjection() const
	{
		return glm::perspectiveFov(glm::radians(mFOV), static_cast<float>(mResolution.x), static_cast<float>(mResolution.y), mNear, mFar);
	}

	void Camera::SetFOV(float value)		{ mFOV = glm::clamp(value, MIN_FOV, MAX_FOV); }
	void Camera::SetNearPlane(float value)	{ mNear = glm::max(value, MIN_NEAR); }
	void Camera::SetFarPlane(float value)	{ mFar = glm::max(value, mNear + MIN_PLANE_GAP); }
	void Camera::SetResolution(glm::uvec2 resolution) { mResolution = glm::max(resolution, MIN_RESOLUTION); }
}