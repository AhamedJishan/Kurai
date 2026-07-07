#pragma once

#include <string>
#include <vector>

// Forward declaration
struct aiMesh;
struct aiMaterial;

namespace Dawn
{
	// Forward declarations
	class RawMesh;
	class RawMaterial;


	class RawModel
	{
	public:
		RawModel() = default;
		~RawModel()
		{
			for (RawMesh* rawMesh : mRawMeshes)
				delete rawMesh;
			for (RawMaterial* rawMaterial : mRawMaterials)
				delete rawMaterial;
		}

		const std::string& GetDirectory() const { return mDirectory; }
		const std::vector<RawMesh*>& GetRawMeshes() const { return mRawMeshes; }
		const std::vector<RawMaterial*>& GetRawMaterials() const { return mRawMaterials; }

		void SetDirectory(const std::string& directory) { mDirectory = directory; }
		void AddRawMesh(RawMesh* rawMesh) { mRawMeshes.emplace_back(rawMesh); }
		void AddRawMaterial(RawMaterial* rawMaterial) { mRawMaterials.emplace_back(rawMaterial); }

		RawModel(const RawModel&) = delete;
		RawModel& operator=(const RawModel&) = delete;

	private:
		std::string mDirectory;

		std::vector<RawMesh*> mRawMeshes;
		std::vector<RawMaterial*> mRawMaterials;
	};
}