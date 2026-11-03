#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include <Dawn/Core/Property.h>
#include <Dawn/Core/Component.h>

namespace Dawn
{
	// Forward declaration
	class Actor;
	class RenderView;
	class Texture;

	class Camera : public Component
	{
	public:
		Camera(Actor* owner);
		~Camera();

		void OnPropertiesChanged() override;

		void Update(float deltaTime) override;

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;

		glm::vec3 UnProject(float screenX, float screenY, float depth);

		// FOV in degrees
		void SetFOV(float value) { mFOV = glm::max(value, 0.0001f); }
		void SetNearPlane(float value) { mNear = value; }
		void SetFarPlane(float value) { mFar = value; }
		void SetResolution(glm::uvec2 resolution);

		// FOV in degrees
		float GetFOV() const { return mFOV; }
		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }
		glm::uvec2 GetResolution() const { return mResolution; }

		void SetRenderToScreen(bool enable);
		bool RendersToScreen() const;
		const Texture* GetOutputTexture() const;

		std::vector<Property> GetProperties() override;

	protected:
		// in degrees
		float mFOV = 60.0f;
		float mNear = 0.01f;
		float mFar = 1000.0f;
		
		bool mRendersToScreen = true;

		glm::uvec2 mResolution;
		std::unique_ptr<RenderView> mRenderView;
	};
}