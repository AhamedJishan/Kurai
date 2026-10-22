#include "RenderTarget.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Dawn/Utils/Log.h>

namespace Dawn
{
	RenderTarget::RenderTarget(const RenderTargetSpecification& spec)
	{
		mResolution = glm::max(spec.resolution, glm::uvec2(1));
		mColorAttachment = new Texture(mResolution.x, mResolution.y, spec.colorAttachment);

		if (spec.depthAttachment.has_value())
			mDepthAttachment = new Texture(mResolution.x, mResolution.y, spec.depthAttachment.value());

		glGenFramebuffers(1, &mFboId);
		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->GetId(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		if (mDepthAttachment)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment->GetId(), 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_ERROR("RenderTarget framebuffer incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	RenderTarget::~RenderTarget()
	{
		if (mFboId != 0)
			glDeleteFramebuffers(1, &mFboId);

		if (mColorAttachment) delete mColorAttachment;
		if (mDepthAttachment) delete mDepthAttachment;
	}

	void RenderTarget::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
	}

	void RenderTarget::SetResolution(glm::uvec2 resolution)
	{
		resolution = glm::max(resolution, glm::uvec2(1));
		if (mResolution == resolution) return;

		mResolution = resolution;
		mColorAttachment->Resize(resolution);
		if (mDepthAttachment) mDepthAttachment->Resize(resolution);
	}
}