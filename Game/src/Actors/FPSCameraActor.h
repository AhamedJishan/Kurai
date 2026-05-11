#pragma once

#include "Core/Actor.h"

#include "Components/FPSCamera.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class FPSCameraActor : public Actor
	{
	public:
		FPSCameraActor(Scene* scene)
			:Actor(scene)
		{
			mCamera = new FPSCamera(this);
			mCamera->SetFOV(80.0f);
		}

		float GetYaw() const { return mCamera->GetYaw(); }
		float GetPitch() const { return mCamera->GetPitch(); }
		float GetFOV() const { return mCamera->GetFOV(); }

		void SetFOV(float fov) { mCamera->SetFOV(fov); }
	private:
		FPSCamera* mCamera = nullptr;
	};
}