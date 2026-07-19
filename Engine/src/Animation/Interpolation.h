#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	enum class Interpolation
	{
		Constant,
		Linear
	};

	template<typename T>
	inline T InterpolateLinear(const T& a, const T& b, float t)
	{
		return glm::mix(a, b, t);
	}

	inline glm::quat InterpolateLinear(const glm::quat& a, const glm::quat& b, float t)
	{
		return glm::slerp(a, b, t);
	}
}