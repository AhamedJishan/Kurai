#pragma once

#include <string>
#include <vector>
#include "Core/Component.h"

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

		void Serialize(YAML::Node& node) const override;
		void Deserialize(const YAML::Node& node) override;

	private:
		bool mIsSkinned = false;
		std::string mModelPath;

		std::vector<Mesh*> mMeshes;
		std::vector<Material*> mMaterials;
	};
}