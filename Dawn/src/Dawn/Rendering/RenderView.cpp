#include "RenderView.h"

namespace Dawn
{
	// 2x2 matches BloomPass’s minimum, so all buffers stay consistent
	static const glm::uvec2 MIN_RESOLUTION = glm::uvec2(2);

	RenderView::RenderView(glm::uvec2 outputResolution, bool renderToScreen)
		:mOutputResolution(glm::max(outputResolution, MIN_RESOLUTION))
	{
		RenderTargetSpecification hdrRTSpec;
		hdrRTSpec.resolution = mRenderResolution;
		hdrRTSpec.colorAttachment = TextureFormat::RGBA16F;
		hdrRTSpec.depthAttachment = TextureFormat::Depth24;

		mHdrTarget = std::make_unique<RenderTarget>(hdrRTSpec);
		mBloomPass = std::make_unique<BloomPass>(6, mRenderResolution);

		SetRenderToScreen(renderToScreen);
	}

	void RenderView::SetOutputResolution(glm::uvec2 outputResolution)
	{
		outputResolution = glm::max(outputResolution, MIN_RESOLUTION);
		if (mOutputResolution == outputResolution) return;
		mOutputResolution = outputResolution;

		if (mOutputTarget)
			mOutputTarget->SetResolution(mOutputResolution);
	}

	void RenderView::SetRenderToScreen(bool enable)
	{
		if (enable)
			mOutputTarget.reset();
		else if (RendersToScreen())
			BuildOutputTarget();
	}

	const Texture* RenderView::GetOutputTexture() const
	{
		if (mOutputTarget != nullptr)
			return mOutputTarget->GetColorTexture();
		return nullptr;
	}

	void RenderView::SetCamera(glm::vec3 position, const glm::mat4& view, const glm::mat4& projection, glm::uvec2 renderResolution)
	{
		mCameraPosition = position;
		mView = view;
		mProjection = projection;
		mRenderResolution = glm::max(renderResolution, MIN_RESOLUTION);

		mHdrTarget->SetResolution(mRenderResolution);
		mBloomPass->SetRenderResolution(mRenderResolution);
	}

	void RenderView::BuildOutputTarget()
	{
		mOutputTarget.reset();
		RenderTargetSpecification outputRTSpec;
		outputRTSpec.resolution = mOutputResolution;
		outputRTSpec.colorAttachment = TextureFormat::RGBA8;
		mOutputTarget = std::make_unique<RenderTarget>(outputRTSpec);
	}
}