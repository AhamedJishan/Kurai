#pragma once

#include <vector>
#include <string>
#include "Core/Vertex.h"

namespace Dawn
{
	class Mesh
	{
	public:
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int rawMaterialIndex);
		~Mesh();

		void Bind() const;

		const std::string& GetName() const { return mName; }
		unsigned int GetIndexCount() const { return mIndexCount; }
		unsigned int GetRawMaterialIndex() const { return mRawMaterialIndex; }

	private:
		void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	private:
		std::string mName;

		unsigned int mIndexCount = 0;
		unsigned int mRawMaterialIndex;

		unsigned int mVAO = 0;
		unsigned int mVBO = 0;
		unsigned int mEBO = 0;

	};
}