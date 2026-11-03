#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "RenderTarget.h"
#include "BloomPass.h"
#include "Texture.h"

namespace Dawn
{
	// used to request Renderer for one complete render call
	class RenderView
	{
	public:
		RenderView(glm::uvec2 resolution, bool renderToScreen, int priority = 0);

		void SetCamera(glm::vec3 viewPosition, glm::mat4 view, glm::mat4 projection);
		void SetResolution(glm::uvec2 resolution);
		void SetRenderToScreen(bool enable);

		const glm::vec3& GetViewPosition() const { return mViewPosition; }
		const glm::mat4& GetViewMatrix() const { return mView; }
		const glm::mat4& GetProjectionMatrix() const { return mProjection; }
		glm::uvec2 GetResolution() const { return mResolution; }
		bool RendersToScreen() const { return mOutputTarget == nullptr; }

		const Texture* GetOutputTexture() const;
		const BloomPass* GetBloomPass() const { return mBloomPass.get(); }
		const RenderTarget* GetHdrRenderTarget() const { return mHdrTarget.get(); }
		const RenderTarget* GetOutputRenderTarget() const { return mOutputTarget.get(); }

	private:
		void BuildOutputTarget();

	private:
		glm::uvec2 mResolution;
		glm::vec3 mViewPosition = glm::vec3(0.0f);
		glm::mat4 mView = glm::mat4(1.0f);
		glm::mat4 mProjection = glm::mat4(1.0f);

		std::unique_ptr<RenderTarget> mHdrTarget = nullptr;
		std::unique_ptr<BloomPass> mBloomPass = nullptr;
		std::unique_ptr<RenderTarget> mOutputTarget = nullptr;
	};
}