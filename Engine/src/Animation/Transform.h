#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	struct Transform
	{
	public:
		Transform()
		{
			Scale = glm::vec3(0.0f);
			Position = glm::vec3(0.0f);
			Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		Transform(const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation)
		{
			Scale = scale;
			Position = position;
			Rotation = rotation;
		}

	public:
		glm::vec3 Scale;
		glm::quat Rotation;
		glm::vec3 Position;
	};
}