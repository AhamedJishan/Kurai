#include "Mesh.h"
#include "Utils/Log.h"

#include <glad/glad.h>

namespace Dawn
{
	Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int rawMaterialIndex)
	{
		mName = name;
		mRawMaterialIndex = rawMaterialIndex;
		mIndexCount = static_cast<unsigned int>(indices.size());

		SetupMesh(vertices, indices);
	}

	Mesh::~Mesh()
	{
		if (mVAO != 0)
			glDeleteVertexArrays(1, &mVAO);
		if (mVBO != 0)
			glDeleteBuffers(1, &mVBO);
		if (mEBO != 0)
			glDeleteBuffers(1, &mEBO);
	}
	
	void Mesh::Bind() const
	{
		if (mVAO != 0)
			glBindVertexArray(mVAO);
		else
			LOG_WARN("Tried to bind an invalid Mesh");
	}

	void Mesh::SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
		glEnableVertexAttribArray(0);
		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
		glEnableVertexAttribArray(1);
		// TexCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoord)));
		glEnableVertexAttribArray(2);
		// Tangents
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangent)));
		glEnableVertexAttribArray(3);

		// Indices
		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}