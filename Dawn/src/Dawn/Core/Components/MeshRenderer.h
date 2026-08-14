#pragma once

#include <string>
#include <vector>
#include "Dawn/Core/Component.h"

namespace Dawn
{
	// Forward declarations
	class Actor;
	class Mesh;
	class Material;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(Actor* owner);
		~MeshRenderer();

		void SetModel(const std::string& modelPath, bool requestSkinning = false);
		const std::string& GetModelPath() const { return mModelPath; }

		const std::vector<Mesh*>& GetMeshes() const { return mMeshes; }
		const std::vector<Material*>& GetMaterials() const { return mMaterials; }

		std::vector<Property> GetProperties() override;
		void OnPropertiesChanged() override;

	private:
		bool mIsSkinned = false;
		std::string mModelPath;

		std::vector<Mesh*> mMeshes;
		std::vector<Material*> mMaterials;
	};
}