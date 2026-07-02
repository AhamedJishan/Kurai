#include "RawModel.h"
#include "Utils/Log.h"

#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec3.hpp>
#include "Vertex.h"
#include "RawMesh.h"
#include "RawMaterial.h"

namespace Dawn
{
	RawModel::RawModel(const std::string& filename)
	{
		mDirectory = filename.substr(0, filename.find_last_of("/\\") + 1);

		if (LoadRawModel(filename))
			mIsValid = true;
		else
			mIsValid = false;
	}

	RawModel::~RawModel()
	{
		for (auto it : mRawMeshes)
			delete it;
		mRawMeshes.clear();

		for (auto it : mRawMaterials)
			delete it;
		mRawMaterials.clear();
	}
	
	bool RawModel::LoadRawModel(const std::string& filename)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename, 
			aiProcess_JoinIdenticalVertices | 
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate );
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("Assimp error: %s", importer.GetErrorString());
			return false;
		}
		// Load Meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			RawMesh* rawMesh = GetRawMesh(mesh);
			mRawMeshes.emplace_back(rawMesh);
		}

		// Load Materials
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial* aiMat = scene->mMaterials[i];
			RawMaterial* rawMaterial = GetRawMaterial(aiMat);
			mRawMaterials.emplace_back(rawMaterial);
		}

		return true;
	}

	RawMesh* RawModel::GetRawMesh(const aiMesh* aiMesh)
	{
		const std::string& name = aiMesh->mName.C_Str();
		int materialIndex = aiMesh->mMaterialIndex;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(aiMesh->mNumVertices);
		for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position.x = aiMesh->mVertices[i].x;
			vertex.Position.y = aiMesh->mVertices[i].y;
			vertex.Position.z = aiMesh->mVertices[i].z;

			vertex.Normal.x = aiMesh->mNormals[i].x;
			vertex.Normal.y = aiMesh->mNormals[i].y;
			vertex.Normal.z = aiMesh->mNormals[i].z;

			if (aiMesh->HasTextureCoords(0))
			{
				vertex.TexCoord.x = aiMesh->mTextureCoords[0][i].x;
				vertex.TexCoord.y = aiMesh->mTextureCoords[0][i].y;
			}

			vertex.Tangent.x = aiMesh->mTangents[i].x;
			vertex.Tangent.y = aiMesh->mTangents[i].y;
			vertex.Tangent.z = aiMesh->mTangents[i].z;

			vertices.emplace_back(vertex);
		}

		indices.reserve(aiMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
		{
			const aiFace& aiFace = aiMesh->mFaces[i];
			for (unsigned int j = 0; j < aiFace.mNumIndices; j++)
			{
				indices.emplace_back(aiFace.mIndices[j]);
			}
		}

		return new RawMesh(name, std::move(vertices), std::move(indices), materialIndex);
	}

	RawMaterial* RawModel::GetRawMaterial(const aiMaterial* aiMat)
	{
		RawMaterial* rawMaterial = new RawMaterial();

		// Shininess
		float shininess;
		if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
			rawMaterial->SetShininess(shininess);
		// Diffuse Color
		aiColor3D diffuseColor;
		if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == aiReturn_SUCCESS)
			rawMaterial->SetDiffuseColor(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));
		// Specular Color
		aiColor3D specularColor;
		if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == aiReturn_SUCCESS)
			rawMaterial->SetSpecularColor(glm::vec3(specularColor.r, specularColor.g, specularColor.b));
		// Emissive Color
		aiColor3D emissiveColor;
		if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == aiReturn_SUCCESS)
			rawMaterial->SetEmissiveColor(glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b));
		
		aiString path;
		// Albedo map
		if (aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == aiReturn_SUCCESS)
			rawMaterial->SetAlbedoTexturePath(mDirectory + path.C_Str());
		// Diffuse map
		if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
			rawMaterial->SetDiffuseTexturePath(mDirectory + path.C_Str());
		// Specular Map
		if (aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path) == aiReturn_SUCCESS)
			rawMaterial->SetSpecularTexturePath(mDirectory + path.C_Str());
		// Normal map
		if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
			rawMaterial->SetNormalTexturePath(mDirectory + path.C_Str());
		// Height Map
		if (aiMat->GetTexture(aiTextureType_HEIGHT, 0, &path) == aiReturn_SUCCESS)
			rawMaterial->SetHeightTexturePath(mDirectory + path.C_Str());
		// Emission Map
		if (aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &path) == aiReturn_SUCCESS)
		{
			rawMaterial->SetEmissiveTexturePath(mDirectory + path.C_Str());
			// NOTE: Temporary fix
			rawMaterial->SetEmissiveColor(glm::vec3(1));
		}

		return rawMaterial;
	}
}