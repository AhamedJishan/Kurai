#pragma once

#include <string>
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Texture.h>
#include <Dawn/Rendering/Shader.h>
#include <Dawn/Rendering/Mesh.h>
#include <Dawn/Animation/Skeleton.h>
#include <Dawn/Animation/Clip.h>
#include "AssetManager.h"
#include "RawModel.h"

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

		static Shader* GetShader(const std::string& shaderName, bool skinned = false)
		{
			return Application::Get()->GetAssetManager()->GetShader(shaderName, skinned);
		}

		static RawModel* GetRawModel(const std::string& path)
		{
			return Application::Get()->GetAssetManager()->GetRawModel(path);
		}

		static const std::vector<Mesh*>& GetMeshes(const std::string& path, bool requestSkinning = false)
		{
			return Application::Get()->GetAssetManager()->GetMeshes(path, requestSkinning);
		}

		static Skeleton* GetSkeleton(const std::string& path)
		{
			return Application::Get()->GetAssetManager()->GetSkeleton(path);
		}

		static Clip* GetAnimationClip(const std::string& path)
		{
			return Application::Get()->GetAssetManager()->GetAnimationClip(path);
		}
	};
}