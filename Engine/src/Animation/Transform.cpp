#include "Transform.h"

namespace Dawn
{
	Transform::Transform()
		:Scale(glm::vec3(1.0f))
		, Position(glm::vec3(0.0f))
		, Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
	{}

	Transform::Transform(const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation)
		:Scale(scale)
		, Position(position)
		, Rotation(rotation)
	{}

	Transform Combine(const Transform& a, const Transform& b)
	{
		Transform result;

		result.Scale = a.Scale * b.Scale;
		result.Rotation = a.Rotation * b.Rotation;
		result.Position = a.Position + a.Rotation * (a.Scale * b.Position);

		return result;
	}

	glm::mat4 TransformToMat4(const Transform& t)
	{
		glm::mat4 result;

		result = glm::scale(glm::mat4(1.0f), t.Scale);
		result = glm::mat4_cast(t.Rotation) * result;
		result = glm::translate(glm::mat4(1.0f), t.Position) * result;

		return result;
	}
}