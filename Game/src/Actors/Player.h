#pragma once

#include "Core/Actor.h"

namespace Dawn
{
	// Forward declarations
	class Scene;
	class FPSCameraActor;

	class Player : public Actor
	{
	public:
		Player(Scene* scene, FPSCameraActor* fpsCameraActor);

		void Update(float deltaTime) override;

		glm::vec3 GetGunPosition();

	private:
		FPSCameraActor* mCamera = nullptr;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);

		glm::vec3 mGunOffset = glm::vec3(0.1f, -.125f, 0.18f);

		float mSpeed = 7.0f;
	};
}