#pragma once

#include <string>
#include "Application.h"
#include "AssetManager.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Core/RawModel.h"

namespace Dawn
{
	// Facade for AssetManager
	class Assets
	{
	public:
		static Texture* GetTexture(const std::string& filePath)
		{
			return Application::Get()->GetAssetManager()->GetTexture(filePath);
		}

		static Shader* GetShader(const std::string& shaderName)
		{
			return Application::Get()->GetAssetManager()->GetShader(shaderName);
		}

		static RawModel* GetRawModel(const std::string& path)
		{
			return Application::Get()->GetAssetManager()->GetRawModel(path);
		}

		static const std::vector<Mesh*>& GetMeshes(const std::string& path)
		{
			return Application::Get()->GetAssetManager()->GetMeshes(path);
		}
	};
}