#pragma once

#include <string>

namespace Dawn
{
	// Forward declarations
	class Scene;

	namespace SceneSerializer
	{
		Scene* Load(const std::string& scenePath);
		// TODO: Save();
	}
}