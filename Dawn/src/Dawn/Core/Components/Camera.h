#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
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

		void OnPropertiesChanged() override;

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;

		// FOV in degrees
		void SetFOV(float value);
		void SetNearPlane(float value);
		void SetFarPlane(float value);
		void SetResolution(glm::uvec2 resolution);

		// FOV in degrees
		float GetFOV() const { return mFOV; }
		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }
		glm::uvec2 GetResolution() const { return mResolution; }

		std::vector<Property> GetProperties() override;

	protected:
		// in degrees
		float mFOV = 60.0f;
		float mNear = 0.01f;
		float mFar = 1000.0f;
		// Scene render resolution. (HDR and PostProcessing buffer. NOT THE FINAL OUTPUT SIZE)
		// Default is 1920x1080
		glm::uvec2 mResolution = {1920u, 1080u};
	};
}