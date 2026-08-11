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

		void Bind();
		void AttachColorTexture(const Texture& colorTexture);
		void AttachDepthTexture(const Texture& depthTexture);

	private:
		unsigned int mFboId = 0;
	};
}