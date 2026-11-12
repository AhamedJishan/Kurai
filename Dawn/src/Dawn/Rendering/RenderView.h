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
		enum class FitMode { Fill, Stretch, Letterbox};

		RenderView(glm::uvec2 outputResolution, bool renderToScreen);

		void SetCamera(glm::vec3 position, const glm::mat4& view, const glm::mat4& projection, glm::uvec2 renderResolution);
		void SetOutputResolution(glm::uvec2 outputResolution);
		void SetRenderToScreen(bool enable);
		void SetFitMode(FitMode fitMode) { mFitMode = fitMode; }

		const glm::vec3& GetCameraPosition() const { return mCameraPosition; }
		const glm::mat4& GetViewMatrix() const { return mView; }
		const glm::mat4& GetProjectionMatrix() const { return mProjection; }
		glm::uvec2 GetOutputResolution() const { return mOutputResolution; }
		glm::uvec2 GetRenderResolution() const { return mRenderResolution; }
		bool RendersToScreen() const { return mOutputTarget == nullptr; }
		FitMode GetFitMode() const { return mFitMode; }

		const Texture* GetOutputTexture() const;
		const BloomPass* GetBloomPass() const { return mBloomPass.get(); }
		const RenderTarget* GetHdrRenderTarget() const { return mHdrTarget.get(); }
		const RenderTarget* GetOutputRenderTarget() const { return mOutputTarget.get(); }

	private:
		void BuildOutputTarget();

	private:
		FitMode mFitMode = FitMode::Fill;
		glm::uvec2 mOutputResolution = { 1920u, 1080u };
		glm::uvec2 mRenderResolution = { 1920u, 1080u };
		glm::vec3 mCameraPosition = glm::vec3(0.0f);
		glm::mat4 mView = glm::mat4(1.0f);
		glm::mat4 mProjection = glm::mat4(1.0f);

		std::unique_ptr<RenderTarget> mHdrTarget = nullptr;
		std::unique_ptr<BloomPass> mBloomPass = nullptr;
		std::unique_ptr<RenderTarget> mOutputTarget = nullptr;
	};
}