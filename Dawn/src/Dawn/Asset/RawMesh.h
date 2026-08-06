#pragma once

#include <string>
#include <vector>
#include <utility>
#include "Vertex.h"

namespace Dawn
{
	// CPU-side mesh data loaded from a model file (no GPU resources).
	class RawMesh
	{
	public:
		RawMesh(const std::string& name, int rawMaterialIndex, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexSkinData> skinData, bool hasSkinData)
			:mName(name)
			,mRawMaterialIndex(rawMaterialIndex)
			,mVertices(std::move(vertices))
			,mIndices(std::move(indices))
			,mSkinData(std::move(skinData))
			,mHasSkinData(hasSkinData)
		{
		}

		const std::string& GetName() const { return mName; }
		int GetRawMaterialIndex() const { return mRawMaterialIndex; }
		bool HasSkinData() const { return mHasSkinData; }
		const std::vector<VertexSkinData>& GetSkinData() const { return mSkinData; }
		const std::vector<Vertex>& GetVertices() const { return mVertices; }
		const std::vector<unsigned int>& GetIndices() const { return mIndices; }

	private:
		std::string mName;
		int mRawMaterialIndex;
		bool mHasSkinData;

		std::vector<Vertex> mVertices;
		std::vector<VertexSkinData> mSkinData;
		std::vector<unsigned int> mIndices;
	};
}