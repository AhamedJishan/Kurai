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
		RawModel(const std::string& filename);
		~RawModel();

		bool IsValid() const { return mIsValid; }

		const std::string& GetDirectory() const { return mDirectory; }
		const std::vector<RawMesh*>& GetRawMeshes() const { return mRawMeshes; }
		const std::vector<RawMaterial*>& GetRawMaterials() const { return mRawMaterials; }

	private:
		bool LoadRawModel(const std::string& filename);
		RawMesh* GetRawMesh(const aiMesh* aiMesh);
		RawMaterial* GetRawMaterial(const aiMaterial* aiMat);

	private:
		bool mIsValid;
		std::string mDirectory;

		std::vector<RawMesh*> mRawMeshes;
		std::vector<RawMaterial*> mRawMaterials;
	};
}