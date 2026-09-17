#pragma once

#include <glm/mat4x4.hpp>
#include <Dawn/Core/Transform.h>

namespace Dawn
{
	class EditorCamera
	{
	public:
		void Update(float deltaTime);

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;

	public:
		// in degrees
		float fov = 60.0f;
		float near = 0.01f;
		float far = 1000.0f;

		Transform transform;
	};
}