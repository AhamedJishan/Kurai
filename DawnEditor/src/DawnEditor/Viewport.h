#pragma once

namespace Dawn
{
	// Forward declarations
	class Texture;

	class Viewport
	{
	public:
		void Draw(Texture* texture);

		bool IsFocused() { return mIsFocused; }
		void SetFocus(bool value = true);
	private:
		const char* mWindowName = "Viewport";
		bool mIsFocused = false;
	};
}