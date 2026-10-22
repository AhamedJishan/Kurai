#pragma once
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declarations
	class Texture;

	class Viewport
	{
	public:
		void Draw(const Texture* texture);

		bool IsFocused() { return mIsFocused; }
		void SetFocus(bool value = true);
		glm::vec2 GetSize() const { return mSize; }

	private:
		const char* mWindowName = "Viewport";
		bool mIsFocused = false;

		glm::vec2 mSize;
	};
}