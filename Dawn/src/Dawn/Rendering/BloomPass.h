#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declarations
	class Shader;
	class Texture;
	class RenderTarget;

	struct Mip
	{
		glm::vec2 size;
		Texture* texture;
	};

	class BloomPass
	{
	public:
		BloomPass(unsigned int mipCount, glm::vec2 renderResolution);
		~BloomPass();

		// quadVAO should be a triangle strip with 4 as the count
		void Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius = 1.0f);

		unsigned int GetBloomTextureId() const;
		void Resize(glm::vec2 renderResolution);

	private:
		void Init(unsigned int mipCount);
		void Shutdown();

		void Downsample(unsigned int hdrTexture, unsigned int quadVAO);
		void Upsample(unsigned int quadVAO);

	private:
		glm::vec2 mRenderResolution;

		RenderTarget* mBloomRenderTarget;
		std::vector<Mip> mMipChain;

		Shader* mUpsampleShader = nullptr;
		Shader* mDownsampleShader = nullptr;
	};
}