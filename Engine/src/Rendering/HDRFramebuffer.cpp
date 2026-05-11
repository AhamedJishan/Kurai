#include "HDRFramebuffer.h"

#include "glad/glad.h"
#include "Core/Application.h"
#include "Core/Window.h"

namespace Dawn
{
	HDRFramebuffer::HDRFramebuffer()
	{
		int width = Application::Get()->GetWindow()->GetWidth();
		int height = Application::Get()->GetWindow()->GetHeight();
		Init(width, height);
	}

	HDRFramebuffer::~HDRFramebuffer()
	{
		glDeleteRenderbuffers(1, &mDepthRBO);
		glDeleteTextures(1, &mColorTexture);
		glDeleteFramebuffers(1, &mId);
	}

	void HDRFramebuffer::BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mId);
	}

	void HDRFramebuffer::BindTexture(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);
	}

	void HDRFramebuffer::Init(int width, int height)
	{
		glGenFramebuffers(1, &mId);
		glBindFramebuffer(GL_FRAMEBUFFER, mId);

		glGenTextures(1, &mColorTexture);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);

		glGenRenderbuffers(1, &mDepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}