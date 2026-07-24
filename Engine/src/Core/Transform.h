#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	struct Transform
	{
	public:
		Transform();
		Transform(const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation);

	public:
		glm::vec3 Scale;
		glm::quat Rotation;
		glm::vec3 Position;
	};

	// In a parent-child relation, a is the parent and b is the child
	Transform Combine(const Transform& a, const Transform& b);
	glm::mat4 TransformToMat4(const Transform& t);
}