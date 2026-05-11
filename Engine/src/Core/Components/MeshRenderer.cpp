#include "MeshRenderer.h"

#include <vector>
#include "Core/Actor.h"
#include "Core/Application.h"
#include "Core/Assets.h"
#include "Core/RawMaterial.h"
#include "Core/RawModel.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/Materials/PhongMaterial.h"

namespace Dawn
{
	MeshRenderer::MeshRenderer(Actor* owner, Mesh* mesh, Material* material, unsigned int updateOrder)
		:Component(owner, updateOrder)
		,mMesh(mesh)
		,mMaterial(material)
	{
		Application::Get()->GetRenderer()->AddMeshRenderer(this);
	}

	MeshRenderer::~MeshRenderer()
	{
		Application::Get()->GetRenderer()->RemoveMeshRenderer(this);

		delete mMaterial;
		// Mesh will be deleted by AssetManager
	}

	void MeshRenderer::CreateFromModel(Actor* owner, const std::string& path)
	{
		RawModel* rawModel = Assets::GetRawModel(path);
		if (!rawModel)
			return;

		const std::vector<Mesh*>& meshes = Assets::GetMeshes(path);
		const std::vector<RawMaterial*>& rawMaterials = rawModel->GetRawMaterials();

		for (Mesh* mesh : meshes)
		{
			int index = mesh->GetRawMaterialIndex();
			if (index < 0 || index >= rawMaterials.size())
				continue;

			PhongMaterial* phongMat = PhongMaterial::CreateFromRaw(rawMaterials[index]);

			new MeshRenderer(owner, mesh, phongMat);
		}
	}
}