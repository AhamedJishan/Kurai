#include "MeshRenderer.h"

#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Asset/Assets.h>
#include <Dawn/Asset/RawMaterial.h>
#include <Dawn/Asset/RawModel.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/Mesh.h>
#include <Dawn/Rendering/Material.h>
#include <Dawn/Rendering/Materials/PhongMaterial.h>

namespace Dawn
{
	std::vector<Property> MeshRenderer::GetProperties()
	{
		return 
		{
			{"ModelPath", &mModelPath, PropertyType::String},
			{"IsSkinned", &mIsSkinned, PropertyType::Bool}
		};
	}

	void MeshRenderer::OnPropertiesChanged()
	{
		SetModel(mModelPath, mIsSkinned);
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