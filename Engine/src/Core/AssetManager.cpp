#include "AssetManager.h"
#include "Utils/Log.h"

#include "Rendering/Image.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Vertex.h"
#include "RawModel.h"
#include "RawMesh.h"


namespace Dawn
{
    AssetManager::AssetManager()
    {
        // Nothing for now
    }

    AssetManager::~AssetManager()
    {
        for (auto& it : mTextures)
            delete it.second;
        mTextures.clear();
        LOG_INFO("Destroyed all Textures");

        for (auto& it : mShaders)
            delete it.second;
        mShaders.clear();
        LOG_INFO("Destroyed all Shaders");

        for (auto& it : mRawModels)
            delete it.second;
        mRawModels.clear();
        LOG_INFO("Destroyed all RawModels");

        for (auto& it : mMeshes)
        {
            for (Mesh* mesh : it.second)
                if (mesh)
                    delete mesh;
        }
        mMeshes.clear();
        LOG_INFO("Destroyed all Meshes");
    }

    Texture* AssetManager::GetTexture(const std::string& filePath)
    {
        auto it = mTextures.find(filePath);
        if (it != mTextures.end())
            return it->second;

        Image image(filePath);
        if (!image.IsValid())
            return nullptr;

        Texture* texture = new Texture(&image);        
        if (!texture->IsValid())
        {
            delete texture;
            return nullptr;
        }

        LOG_INFO("Loaded Texture: %s", filePath.c_str());
        mTextures.emplace(filePath, texture);

        return texture;
    }

    Shader* AssetManager::GetShader(const std::string& shaderName)
    {
        auto it = mShaders.find(shaderName);
        if (it != mShaders.end())
            return it->second;

        Shader* shader = CreateShaderByName(shaderName);

        if (!shader)
        {
            LOG_ERROR("'%s' shader doesn't exist", shaderName.c_str());
            return nullptr;
        }
            
        mShaders[shaderName] = shader;
        return shader;
    }

    RawModel* AssetManager::GetRawModel(const std::string& path)
    {
        auto it = mRawModels.find(path);
        if (it != mRawModels.end())
            return it->second;

        RawModel* rawModel = new RawModel(path);
        if (!rawModel || !rawModel->IsValid())
        {
            delete rawModel;
            return nullptr;
        }

        mRawModels.emplace(path, rawModel);

        return rawModel;
    }

    const std::vector<Mesh*>& AssetManager::GetMeshes(const std::string& path)
    {
        auto it = mMeshes.find(path);
        if (it != mMeshes.end())
            return it->second;

        RawModel* rawModel = GetRawModel(path);
        if (!rawModel)
        {
            // cache an empty result
            mMeshes.emplace(path, std::vector<Mesh*>{});
            return mMeshes.at(path);
        }

        const std::vector<RawMesh*>& rawMeshes = rawModel->GetRawMeshes();

        std::vector<Mesh*> meshes;
        meshes.reserve(rawMeshes.size());
        for (size_t i = 0; i < rawMeshes.size(); i++)
        {
            const std::string& name = rawMeshes[i]->GetName();
            const std::vector<Vertex>& vertices = rawMeshes[i]->GetVertices();
            const std::vector<unsigned int>& indices = rawMeshes[i]->GetIndices();
            unsigned int rawMaterialIndex = rawMeshes[i]->GetRawMaterialIndex();
            Mesh* mesh = new Mesh(name, vertices, indices, rawMaterialIndex);

            meshes.emplace_back(mesh);
        }

        mMeshes.emplace(path, std::move(meshes));

        return mMeshes.at(path);
    }

    Shader* AssetManager::CreateShaderByName(const std::string& name)
    {
        Shader* shader = nullptr;

        shader = new Shader("Assets/Shaders/" + name + ".vert", "Assets/Shaders/" + name + ".frag");

        if (!shader || !shader->IsValid())
        {
            delete shader;
            return nullptr;
        }

        return shader;
    }
}