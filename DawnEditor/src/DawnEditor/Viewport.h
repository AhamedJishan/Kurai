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
	private:
		bool mIsFocused = false;
	};
}