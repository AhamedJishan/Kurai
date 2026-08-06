#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Dawn
{
	struct Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		glm::vec3 Tangent;
	};

	struct VertexSkinData
	{
	public:
		void AddWeight(int boneId, float weight)
		{
			if (Weights[0] == 0.0f)
			{
				Ids[0] = boneId;
				Weights[0] = weight;
				return;
			}

			int smallestWeightIndex = 0;

			for (int i = 0; i < 4; i++)
				if (Weights[i] < Weights[smallestWeightIndex])
					smallestWeightIndex = i;

			Ids[smallestWeightIndex] = boneId;
			Weights[smallestWeightIndex] = weight;
		}

	public:
		glm::ivec4 Ids = glm::ivec4(0);
		glm::vec4 Weights = glm::vec4(0.0f);
	};
}