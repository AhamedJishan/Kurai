#include "RenderView.h"

namespace Dawn
{
	// 2x2 matches BloomPass’s minimum, so all buffers stay consistent
	static const glm::uvec2 MIN_RESOLUTION = glm::uvec2(2);

	RenderView::RenderView(glm::uvec2 resolution, bool renderToScreen, int priority)
		:mResolution(glm::max(resolution, MIN_RESOLUTION))
	{
		RenderTargetSpecification hdrRTSpec;
		hdrRTSpec.resolution = glm::uvec2(mResolution);
		hdrRTSpec.colorAttachment = TextureFormat::RGBA16F;
		hdrRTSpec.depthAttachment = TextureFormat::Depth24;
		mHdrTarget = std::make_unique<RenderTarget>(hdrRTSpec);

		mBloomPass = std::make_unique<BloomPass>(6, mResolution);

		if (!renderToScreen)
			BuildOutputTarget();
	}

	void RenderView::SetResolution(glm::uvec2 resolution)
	{
		resolution = glm::max(resolution, MIN_RESOLUTION);
		if (mResolution == resolution) return;
		mResolution = resolution;

		mHdrTarget->SetResolution(mResolution);
		mBloomPass->SetRenderResolution(mResolution);
		if (mOutputTarget)
			mOutputTarget->SetResolution(mResolution);
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

	void RenderView::SetCamera(glm::vec3 viewPosition, glm::mat4 view, glm::mat4 projection)
	{
		mViewPosition = viewPosition;
		mView = view;
		mProjection = projection;
	}

	void RenderView::BuildOutputTarget()
	{
		mOutputTarget.reset();
		RenderTargetSpecification outputRTSpec;
		outputRTSpec.resolution = glm::uvec2(mResolution);
		outputRTSpec.colorAttachment = TextureFormat::RGBA8;
		mOutputTarget = std::make_unique<RenderTarget>(outputRTSpec);
	}
}