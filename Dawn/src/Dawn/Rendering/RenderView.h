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
	class RenderView
	{
	public:
		RenderView(glm::uvec2 resolution, bool renderToScreen, int priority = 0);

		void Update(glm::vec3 viewPosition, glm::mat4 view, glm::mat4 projection);

		void SetEnabled(bool enabled) { mEnabled = enabled; }
		void SetResolution(glm::uvec2 resolution);
		void SetRenderToScreen(bool enable);

		bool IsEnabled() const { return mEnabled; }
		int GetPriority() const { return mPriority; }
		glm::uvec2 GetResolution() const { return mResolution; }
		bool RendersToScreen() const { return mOutputTarget == nullptr; }
		const Texture* GetOutputTexture() const;

	private:
		friend class Renderer;
		// TODO: TBDL

		void BuildOutputTarget();

	private:
		int mPriority = 0;
		bool mEnabled = true;

		glm::uvec2 mResolution;
		glm::vec3 mViewPosition = glm::vec3(0.0f);
		glm::mat4 mView = glm::mat4(1.0f);
		glm::mat4 mProjection = glm::mat4(1.0f);

		std::unique_ptr<RenderTarget> mHdrTarget = nullptr;
		std::unique_ptr<BloomPass> mBloomPass = nullptr;
		std::unique_ptr<RenderTarget> mOutputTarget = nullptr;
	};
}