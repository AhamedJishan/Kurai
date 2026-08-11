#include "RenderTarget.h"

#include <glad/glad.h>
#include <Dawn/Utils/Log.h>
#include "Texture.h"

namespace Dawn
{
	RenderTarget::RenderTarget()
	{
		glGenFramebuffers(1, &mFboId);
	}

	RenderTarget::~RenderTarget()
	{
		if (mFboId != 0)
			glDeleteFramebuffers(1, &mFboId);
	}

	void RenderTarget::AttachColorTexture(const Texture& colorTexture)
	{
		if (!colorTexture.IsValid())
		{
			LOG_ERROR("Invalid color texture attachment");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.GetId(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}

	void RenderTarget::AttachDepthTexture(const Texture& depthTexture)
	{
		if (!depthTexture.IsValid())
		{
			LOG_ERROR("Invalid depth texture attachment");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.GetId(), 0);
	}

	void RenderTarget::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
	}
}