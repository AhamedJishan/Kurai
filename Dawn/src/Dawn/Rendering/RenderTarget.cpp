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

	void RenderTarget::Bind(const Texture* colorAttachment, const Texture* depthAttachment)
	{
		if (!colorAttachment || !colorAttachment->IsValid())
		{
			LOG_ERROR("Invalid Color Texture Attachment to Render Target!");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment->GetId(), 0);

		if (depthAttachment && depthAttachment->IsValid())
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->GetId(), 0);
		else
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			LOG_ERROR("RenderTarget Framebuffer is invalid");

	}
}