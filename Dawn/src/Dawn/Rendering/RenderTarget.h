#pragma once

#include <optional>
#include <glm/vec2.hpp>
#include "Texture.h"

namespace Dawn
{
	struct RenderTargetSpecification
	{
		glm::uvec2 resolution = { 1, 1 };
		TextureFormat colorAttachment = TextureFormat::RGBA8;
		std::optional<TextureFormat> depthAttachment;
	};

	class RenderTarget
	{
	public:
		RenderTarget(const RenderTargetSpecification& spec);
		~RenderTarget();
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		void Bind() const;
		void SetResolution(glm::uvec2 resolution);
		glm::uvec2 GetResolution() const { return mResolution; }
		const Texture* GetColorTexture() const { return mColorAttachment; }

	private:
		glm::uvec2 mResolution;

		unsigned int mFboId = 0;
		Texture* mColorAttachment = nullptr;
		Texture* mDepthAttachment = nullptr;
	};
}