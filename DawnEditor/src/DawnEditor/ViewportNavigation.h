#pragma once

#include "Viewport.h"
#include "EditorCamera.h"

namespace Dawn
{
	class ViewportNavigation
	{
	public:
		void Update(float deltaTime, Viewport& viewport, EditorCamera& camera);

	private:
		float mCameraMoveSpeed = 10.0f;
		float mCameraSlowMoveMultiplier = 0.25f;
	};
}