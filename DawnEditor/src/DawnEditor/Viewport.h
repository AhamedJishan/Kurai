#pragma once
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declarations
	class Texture;

	class Viewport
	{
	public:
		Viewport(const char* title);

		void DrawTexture(const Texture* texture);
		void DrawText(const char* text);

		bool IsVisible() const { return mIsVisible; }
		bool IsFocused() const { return mIsFocused; }
		void SetFocus();
		glm::vec2 GetSize() const { return mSize; }

	private:
		bool Begin();
		void End();

	private:
		const char* mWindowName = "--UNTITLED--";
		bool mIsVisible = true;
		bool mIsFocused = false;

		glm::vec2 mSize = { 1920, 1080 };
	};
}