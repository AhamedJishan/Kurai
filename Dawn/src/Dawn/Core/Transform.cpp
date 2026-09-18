#include "Transform.h"

namespace Dawn
{
	Transform::Transform()
		: scale(glm::vec3(1.0f))
		, position(glm::vec3(0.0f))
		, rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
	{}

	Transform::Transform(const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation)
		: scale(scale)
		, position(position)
		, rotation(rotation)
	{}

	glm::mat4 Transform::ToMatrix()
	{
		glm::mat4 result;

		result = glm::scale(glm::mat4(1.0f), scale);
		result = glm::mat4_cast(rotation) * result;
		result = glm::translate(glm::mat4(1.0f), position) * result;

		return result;
	}

	void Transform::Rotate(float angle, const glm::vec3& axisOfRotation)
	{
		rotation = glm::normalize(rotation * glm::angleAxis(angle, axisOfRotation));
	}

	Transform Transform::Combine(const Transform& a, const Transform& b)
	{
		Transform result;

		result.scale = a.scale * b.scale;
		result.rotation = a.rotation * b.rotation;
		result.position = a.position + a.rotation * (a.scale * b.position);

		return result;
	}
}