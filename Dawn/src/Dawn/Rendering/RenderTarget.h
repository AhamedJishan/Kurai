#pragma once

namespace Dawn
{
	// Forward declarations
	class Texture;

	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		void Bind(const Texture* colorAttachment, const Texture* depthAttachment = nullptr);

	private:
		unsigned int mFboId = 0;
	};
}