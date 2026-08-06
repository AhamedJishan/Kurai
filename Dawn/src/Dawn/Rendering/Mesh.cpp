#include "Mesh.h"
#include "Dawn/Utils/Log.h"

#include <glad/glad.h>
#include <Dawn/Asset/RawMesh.h>

namespace Dawn
{
	Mesh::Mesh(const RawMesh* rawMesh, MeshType meshType)
	{
		mName = rawMesh->GetName();
		mMeshType = meshType;
		mMaterialIndex = rawMesh->GetRawMaterialIndex();
		mIndexCount = static_cast<unsigned int>(rawMesh->GetIndices().size());

		if (mMeshType == MeshType::Skinned && !rawMesh->HasSkinData())
		{
			LOG_WARN("Mesh '%s' was requested to be skinned but lacks skin data, loading it as static mesh!", mName.c_str());
			mMeshType = MeshType::Static;
		}

		SetupMesh(rawMesh->GetVertices(), rawMesh->GetIndices(), rawMesh->GetSkinData());
	}

	Mesh::~Mesh()
	{
		if (mVAO != 0)
			glDeleteVertexArrays(1, &mVAO);
		if (mVertexVBO != 0)
			glDeleteBuffers(1, &mVertexVBO);
		if (mSkinDataVBO != 0)
			glDeleteBuffers(1, &mSkinDataVBO);
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

	void Mesh::SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<VertexSkinData>& skinData)
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
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
		// Skin Data
		if (mMeshType == MeshType::Skinned)
		{
			glGenBuffers(1, &mSkinDataVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mSkinDataVBO);
			glBufferData(GL_ARRAY_BUFFER, skinData.size() * sizeof(VertexSkinData), skinData.data(), GL_STATIC_DRAW);
			glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexSkinData), (void*)(offsetof(VertexSkinData, Ids)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexSkinData), (void*)(offsetof(VertexSkinData, Weights)));
			glEnableVertexAttribArray(5);
		}

		// Indices
		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}