#include "MeshRenderer.h"

#include <yaml-cpp/yaml.h>
#include "Core/Actor.h"
#include "Core/Application.h"
#include "Asset/Assets.h"
#include "Asset/RawMaterial.h"
#include "Asset/RawModel.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/Materials/PhongMaterial.h"

namespace Dawn
{
	void MeshRenderer::Serialize(YAML::Node& node, SerializationContext& serializationContext) const
	{
		node["IsSkinned"] = mIsSkinned;
		node["ModelPath"] = mModelPath;
	}
	
	void MeshRenderer::Deserialize(const YAML::Node & node, SerializationContext& serializationContext)
	{
		SetModel(node["ModelPath"].as<std::string>(), node["IsSkinned"].as<bool>());
	}

	MeshRenderer::MeshRenderer(Actor* owner)
		:Component(owner)
	{
		Application::Get()->GetRenderer()->AddMeshRenderer(this);
	}

	MeshRenderer::~MeshRenderer()
	{
		Application::Get()->GetRenderer()->RemoveMeshRenderer(this);
		
		for (Material* mat : mMaterials)
			delete mat;
		mMaterials.clear();
	}

	void MeshRenderer::SetModel(const std::string& modelPath, bool requestSkinning)
	{
		RawModel* rawModel = Assets::GetRawModel(modelPath);
		if (!rawModel)
			return;

		mModelPath = modelPath;
		mIsSkinned = requestSkinning;

		if (mMaterials.size() > 0)
		{
			for (Material* mat : mMaterials)
				delete mat;
			mMaterials.clear();
		}

		const std::vector<Mesh*>& meshes = Assets::GetMeshes(modelPath, requestSkinning);
		const std::vector<RawMaterial*>& rawMaterials = rawModel->GetRawMaterials();

		mMeshes = meshes;
		mMaterials.reserve(rawMaterials.size());
		for (RawMaterial* rawMaterial : rawMaterials)
			mMaterials.push_back(PhongMaterial::CreateFromRaw(rawMaterial));
	}
}