#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <Dawn/Core/Property.h>
#include <Dawn/Core/Component.h>

namespace Dawn
{
	// Forward declaration
	class Actor;

	class Camera : public Component
	{
	public:
		Camera(Actor* owner);

		void Update(float deltaTime) override;

		glm::mat4 GetView() const;

		glm::vec3 UnProject(float screenX, float screenY, float depth);
		
		// FOV in degrees
		void SetFOV(float value) { mFOV = value; }
		void SetNearPlane(float value) { mNear = value; }
		void SetFarPlane(float value) { mFar = value; }

		// FOV in degrees
		float GetFOV() const { return mFOV; }
		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }

		std::vector<Property> GetProperties() override;

	protected:
		// in degrees
		float mFOV = 60.0f;
		float mNear = 0.01f;
		float mFar = 1000.0f;
	};
}