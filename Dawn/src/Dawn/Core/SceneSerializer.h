#pragma once

#include <filesystem>

namespace Dawn
{
	// Forward declarations
	class Scene;

	namespace SceneSerializer
	{
		Scene* Load(const std::filesystem::path& scenePath);
		bool Save();
	}
}