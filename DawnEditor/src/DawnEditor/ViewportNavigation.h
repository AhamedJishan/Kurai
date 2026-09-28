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
		const float mCameraMoveSpeed = 10.0f;
		const float mCameraSlowMoveMultiplier = 0.25f;

		const float mCameraLookSpeed = 1.5f;
		float mCameraYaw = 0.0f;
		float mCameraPitch = 0.0f;
	};
}