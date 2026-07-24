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

		glm::mat4 ToMatrix();

		// Angle in radians
		void Rotate(float angle, const glm::vec3& axisOfRotation);
		glm::vec3 GetUp()		const { return glm::normalize(glm::mat3_cast(Rotation) * glm::vec3(0, 1, 0)); }
		glm::vec3 GetRight()	const { return glm::normalize(glm::mat3_cast(Rotation) * glm::vec3(1, 0, 0)); }
		glm::vec3 GetForward()	const { return glm::normalize(glm::mat3_cast(Rotation) * glm::vec3(0, 0, -1)); }

		// In a parent-child relation, a is the parent and b is the child
		static Transform Combine(const Transform& a, const Transform& b);

	public:
		glm::vec3 Scale;
		glm::quat Rotation;
		glm::vec3 Position;
	};
}