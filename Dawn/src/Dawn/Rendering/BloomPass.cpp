#include "BloomPass.h"
#include "Dawn/Utils/Log.h"

#include <glad/glad.h>
#include "Dawn/Core/Application.h"
#include "Dawn/Core/Scene.h"
#include "Dawn/Core/Window.h"
#include "Dawn/Asset/Assets.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTarget.h"

namespace Dawn
{
	BloomPass::BloomPass(unsigned int mipCount, glm::vec2 renderResolution)
		:mRenderResolution(renderResolution)
	{
		Init(mipCount);
	}

	BloomPass::~BloomPass()
	{
		Shutdown();
	}
	
	void BloomPass::Init(unsigned int mipCount)
	{
		mBloomRenderTarget = new RenderTarget();

		glm::vec2 mipSize = mRenderResolution / 2.0f;

		// Create Mips
		for (unsigned int i = 0; i < mipCount; i++)
		{
			if (mipSize.x < 2.0f || mipSize.y < 2.0f)
				break;

			Mip mip;
			mip.size = mipSize;

			Texture* mipTexture = new Texture(static_cast<int>(mip.size.x), static_cast<int>(mip.size.y), TextureFormat::R11G11B10F);
			
			mip.texture = mipTexture;
			mMipChain.push_back(mip);

			mipSize = mipSize / 2.0f;
		}

		LOG_INFO("BloomPass initialized: %d mip count", mMipChain.size());

		mUpsampleShader = Assets::GetShader("bloom_upsample");
		mDownsampleShader = Assets::GetShader("bloom_downsample");
	}
	
	void BloomPass::Shutdown()
	{
		for (Mip& mip : mMipChain)
			if (mip.texture)
				delete mip.texture;
		mMipChain.clear();

		delete mBloomRenderTarget;
	}

	void BloomPass::Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius)
	{
		Downsample(hdrTexture, quadVAO);
		Upsample(quadVAO);

		glViewport(0, 0, mRenderResolution.x, mRenderResolution.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int BloomPass::GetBloomTextureId() const
	{
		return mMipChain[0].texture->GetId();
	}

	void BloomPass::Resize(glm::vec2 renderResolution)
	{
		glm::vec2 mipSize = renderResolution / 2.0f;
		for (Mip& mip : mMipChain)
		{
			if (mipSize.x < 2.0f || mipSize.y < 2.0f)
				mipSize = glm::vec2(2.0f);
			mip.size = mipSize;
			mip.texture->SetSize(mipSize.x, mipSize.y);
			mipSize /= 2.0f;
		}
	}

	void BloomPass::Downsample(unsigned int hdrTexture, unsigned int quadVAO)
	{
		mDownsampleShader->Bind();

		glDisable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		mDownsampleShader->SetInt("u_SrcTexture", 0);
		mDownsampleShader->SetVec2("u_SrcResolution", mRenderResolution);

		mBloomRenderTarget->Bind();
		for (int i = 0; i < mMipChain.size(); i++)
		{
			mBloomRenderTarget->AttachColorTexture(*mMipChain[i].texture);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[i].texture->GetId(), 0);
			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// For the next iteration
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i].texture->GetId());
			mDownsampleShader->SetInt("u_SrcTexture", 0);
			mDownsampleShader->SetVec2("u_SrcResolution", mMipChain[i].size);
		}
	}

	void BloomPass::Upsample(unsigned int quadVAO)
	{
		mUpsampleShader->Bind();
		mUpsampleShader->SetFloat("u_BloomRadius", Application::Get()->GetScene()->GetEnvironmentSettings().bloomRadius);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);

		mBloomRenderTarget->Bind();
		for (int i = mMipChain.size() - 2; i >= 0; i--)
		{
			mBloomRenderTarget->AttachColorTexture(*mMipChain[i].texture);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[i].texture->GetId(), 0);
			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i + 1].texture->GetId());
			mUpsampleShader->SetInt("u_SrcTexture", 0);
			mUpsampleShader->SetVec2("u_SrcResolution", mMipChain[i + 1].size);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}