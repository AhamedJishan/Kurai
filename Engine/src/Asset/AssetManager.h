#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace Dawn
{
	// Forward declarations
	class Texture;
	class Shader;
	class RawModel;
	class Mesh;

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		Texture* GetTexture(const std::string& filePath);
		Shader* GetShader(const std::string& shaderName, bool skinned);
		RawModel* GetRawModel(const std::string& path);
		const std::vector<Mesh*>& GetMeshes(const std::string& path);

	private:
		Shader* CreateShaderByName(const std::string& name, bool skinned);

	private:
		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, Shader*> mShaders;
		std::unordered_map<std::string, RawModel*> mRawModels;
		std::unordered_map<std::string, std::vector<Mesh*>> mMeshes;
	};
}