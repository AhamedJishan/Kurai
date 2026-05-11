#pragma once

#include "Core/Actor.h"
#include "Core/Components/Camera.h"

namespace Dawn
{
	class DummyCamera : public Actor
	{
	public:
		DummyCamera(class Scene* scene)
			:Actor(scene)
		{
			mCamera = new Camera(this);
			mCamera->SetFOV(80.0f);
		}

	private:
		Camera* mCamera = nullptr;
	};
}