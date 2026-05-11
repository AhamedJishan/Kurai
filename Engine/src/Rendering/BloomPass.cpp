#include "BloomPass.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/Window.h"
#include "Core/Assets.h"
#include "Shader.h"

namespace Dawn
{
	BloomPass::BloomPass(unsigned int mipCount)
	{
		Init(mipCount);
	}

	BloomPass::~BloomPass()
	{
		Shutdown();
	}
	
	void BloomPass::Init(unsigned int mipCount)
	{
		mSourceWidth;
		mSourceHeight;
		Application::Get()->GetWindow()->GetFrameBufferSize(mSourceWidth, mSourceHeight);

		// Init FBO
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glm::vec2 mipSize = glm::vec2(mSourceWidth / 2.0f, mSourceHeight / 2.0f);

		// Create Mips
		for (unsigned int i = 0; i < mipCount; i++)
		{
			if (mipSize.x < 2.0f || mipSize.y < 2.0f)
				break;

			Mip mip;
			mip.size = mipSize;

			glGenTextures(1, &mip.texture);
			glBindTexture(GL_TEXTURE_2D, mip.texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, 
				static_cast<GLsizei>(mip.size.x), static_cast<GLsizei>(mip.size.y), 
				0, GL_RGB, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			mMipChain.push_back(mip);
		}

		// Check FrameBuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[0].texture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERROR("BloomPass FBO incomplete: 0x%x", status);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}

		LOG_INFO("BloomPass initialized: %d mip count", mMipChain.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		mUpsampleShader = Assets::GetShader("bloom_upsample");
		mDownsampleShader = Assets::GetShader("bloom_downsample");
	}
	
	void BloomPass::Shutdown()
	{
		for (Mip& mip : mMipChain)
			if (mip.texture)
				glDeleteTextures(1, &mip.texture);
		mMipChain.clear();

		if (mFBO)
			glDeleteFramebuffers(1, &mFBO);
	}

	void BloomPass::Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		Downsample(hdrTexture, quadVAO);
		Upsample(quadVAO, bloomRadius);

		glViewport(0, 0, mSourceWidth, mSourceHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void BloomPass::Downsample(unsigned int hdrTexture, unsigned int quadVAO)
	{
		mDownsampleShader->Bind();

		glDisable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		mDownsampleShader->SetInt("u_SrcTexture", 0);
		mDownsampleShader->SetVec2("u_SrcResolution", glm::vec2(mSourceWidth, mSourceHeight));

		for (int i = 0; i < mMipChain.size(); i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[i].texture, 0);
			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// For the next iteration
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i].texture);
			mDownsampleShader->SetInt("u_SrcTexture", 0);
			mDownsampleShader->SetVec2("u_SrcResolution", mMipChain[i].size);
		}
	}

	void BloomPass::Upsample(unsigned int quadVAO, float bloomRadius)
	{
		mUpsampleShader->Bind();
		mUpsampleShader->SetFloat("u_BloomRadius", Application::Get()->GetScene()->GetEnvironmentSettings().bloomRadius);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);

		for (int i = mMipChain.size() - 2; i >= 0; i--)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[i].texture, 0);
			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i + 1].texture);
			mUpsampleShader->SetInt("u_SrcTexture", 0);
			mUpsampleShader->SetVec2("u_SrcResolution", mMipChain[i + 1].size);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}