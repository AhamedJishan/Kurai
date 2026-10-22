#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declarations
	class Shader;
	class Texture;
	class RenderTarget;

	class BloomPass
	{
	public:
		struct Mip
		{
			glm::uvec2 size;
			RenderTarget* renderTarget;
		};

		BloomPass(unsigned int mipCount, glm::uvec2 renderResolution);
		~BloomPass();
		BloomPass(const BloomPass&) = delete;
		BloomPass& operator=(const BloomPass&) = delete;

		// Changes the glViewport, so reset it after this call
		// quadVAO should be a triangle strip with 4 as the count
		void Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius = 1.0f);

		unsigned int GetBloomTextureId() const;
		void SetRenderResolution(glm::uvec2 renderResolution);

	private:
		void Init();
		void Shutdown();
		void BuildMipChain();

		void Downsample(unsigned int hdrTexture, unsigned int quadVAO);
		void Upsample(unsigned int quadVAO, float bloomRadius);

	private:
		unsigned int mMipCount;
		glm::uvec2 mRenderResolution;

		std::vector<Mip> mMipChain;

		Shader* mUpsampleShader = nullptr;
		Shader* mDownsampleShader = nullptr;
	};
}