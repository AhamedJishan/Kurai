#pragma once

#include <vector>
#include <string>
#include "Asset/Vertex.h"

namespace Dawn
{
	// Forward declarations
	class RawMesh;

	enum class MeshType { Static, Skinned };

	class Mesh
	{
	public:
		Mesh(const RawMesh* rawMesh, MeshType meshType);
		~Mesh();

		void Bind() const;

		const std::string& GetName() const { return mName; }
		MeshType GetMeshType() const { return mMeshType; }
		unsigned int GetIndexCount() const { return mIndexCount; }
		unsigned int GetRawMaterialIndex() const { return mRawMaterialIndex; }

	private:
		void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<VertexSkinData>& skinData);

	private:
		std::string mName;
		MeshType mMeshType;

		unsigned int mIndexCount = 0;
		unsigned int mRawMaterialIndex;

		unsigned int mVAO = 0;
		unsigned int mEBO = 0;
		unsigned int mVertexVBO = 0;
		unsigned int mSkinDataVBO = 0;

	};
}