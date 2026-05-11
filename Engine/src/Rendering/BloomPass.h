#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declarations
	class Shader;

	struct Mip
	{
		glm::vec2 size;
		unsigned int texture;
	};

	class BloomPass
	{
	public:
		BloomPass(unsigned int mipCount = 6);
		~BloomPass();

		// quadVAO should be a triangle strip with 4 as the count
		void Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius = 1.0f);

		unsigned int GetBloomTextureId() const { return mMipChain[0].texture; }

	private:
		void Init(unsigned int mipCount);
		void Shutdown();

		void Downsample(unsigned int hdrTexture, unsigned int quadVAO);
		void Upsample(unsigned int quadVAO, float bloomRadius);

	private:
		unsigned int mFBO = 0;
		int mSourceWidth = 0;
		int mSourceHeight = 0;

		std::vector<Mip> mMipChain;

		Shader* mUpsampleShader = nullptr;
		Shader* mDownsampleShader = nullptr;
	};
}