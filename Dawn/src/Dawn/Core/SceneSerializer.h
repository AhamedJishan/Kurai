#pragma once

#include <filesystem>
#include <string>

namespace Dawn
{
	// Forward declarations
	class Scene;

	namespace SceneSerializer
	{
		bool Serialize(const Scene* scene, const std::filesystem::path& scenePath);
		Scene* Deserialize(const std::filesystem::path& scenePath);

		bool Serialize(const Scene* scene, std::string& outSerializedScene);
		Scene* Deserialize(const std::string& serializedScene);
	}
}