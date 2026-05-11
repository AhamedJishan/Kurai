#pragma once

#include <glm/vec3.hpp>

namespace Dawn
{
	namespace Physics
	{
		// Direction should always be normalized
		struct Ray
		{
			glm::vec3 origin = glm::vec3(0);
			glm::vec3 direction = glm::vec3(0, 0, -1);;
		};

		struct Sphere
		{
			glm::vec3 center = glm::vec3(0);
			float radius = 1.0f;
		};


		bool Intersects(const Sphere& a, const Sphere& b);
		bool Intersects(const Ray& ray, const Sphere& sphere, float& outT);
	}
}