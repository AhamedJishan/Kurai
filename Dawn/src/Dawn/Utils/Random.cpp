#include "Random.h"

namespace Dawn
{
	namespace Random
	{
		std::mt19937& GetEngine()
		{
			static std::mt19937 engine(std::random_device{}());
			return engine;
		}

		float Float(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(GetEngine());
		}
		
		int Int(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(GetEngine());
		}
		
		glm::vec3 Vec3(glm::vec3 min, glm::vec3 max)
		{
			return glm::vec3(
				Float(min.x, max.x),
				Float(min.y, max.y),
				Float(min.z, max.z)
			);
		}
	}
}