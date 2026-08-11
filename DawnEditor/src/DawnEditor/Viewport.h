#pragma once

namespace Dawn
{
	// Forward declarations
	class Texture;

	namespace Editor
	{
		void BeginViewport(Texture* viewportTexture);
		void EndViewport();
	}
}