#pragma once

#include <random>
#include <glm/vec3.hpp>

namespace Dawn
{
	namespace Random
	{
		std::mt19937& GetEngine();

		float Float(float min, float max);
		int Int(int min, int max);
		glm::vec3 Vec3(glm::vec3 min, glm::vec3 max);
	}
}