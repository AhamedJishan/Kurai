#pragma once

#include <glm/vec3.hpp>
#include "Viewport.h"
#include "EditorCamera.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class ViewportNavigation
	{
	public:
		void Update(float deltaTime, Viewport& viewport, EditorCamera& camera, Actor* selectedActor = nullptr);

	private:
		const float mCameraMoveSpeed = 10.0f;
		const float mCameraSlowMoveMultiplier = 0.25f;

		const float mCameraLookSpeed = 1.5f;
		float mCameraYaw = 0.0f;
		float mCameraPitch = 0.0f;

		const float mActorFocusDistance = 10.0f;
		const float mActorFocusSpeed = 7.5f;
		bool mShouldFocusActor = false;
		glm::vec3 mActorFocusPosition = { 0, 0, 0 };
	};
}