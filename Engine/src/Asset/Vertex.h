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

	struct VertexBoneData
	{
	public:
		glm::ivec4 BoneIds;
		glm::vec4 BoneWeights;
	};
}