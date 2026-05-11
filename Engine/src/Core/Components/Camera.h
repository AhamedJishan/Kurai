#pragma once

#include "Core/Component.h"
#include <glm/mat4x4.hpp>

namespace Dawn
{
	// Forward declaration
	class Actor;

	class Camera : public Component
	{
	public:
		Camera(Actor* owner, unsigned int updateOrder = 100);

		void Update(float deltaTime) override;

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;

		glm::vec3 UnProject(float screenX, float screenY, float depth);
		
		// FOV in degrees
		void SetFOV(float value) { mFOV = value; }
		void SetNearPlane(float value) { mNear = value; }
		void SetFarPlane(float value) { mFar = value; }

		// FOV in degrees
		float GetFOV() const { return mFOV; }
		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }

	protected:
		// in degrees
		float mFOV = 60.0f;
		float mNear = 0.01f;
		float mFar = 1000.0f;
	};
}