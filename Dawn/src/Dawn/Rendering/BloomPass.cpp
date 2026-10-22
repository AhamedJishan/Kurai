#include "BloomPass.h"
#include "Dawn/Utils/Log.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Dawn/Asset/Assets.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTarget.h"

namespace Dawn
{
	BloomPass::BloomPass(unsigned int mipCount, glm::uvec2 renderResolution)
		:mMipCount(mipCount)
	{
		mRenderResolution = glm::max(renderResolution, glm::uvec2(2, 2));
		Init();
	}

	BloomPass::~BloomPass()
	{
		Shutdown();
	}
	
	void BloomPass::Init()
	{
		BuildMipChain();

		mUpsampleShader = Assets::GetShader("bloom_upsample");
		mDownsampleShader = Assets::GetShader("bloom_downsample");

		LOG_INFO("BloomPass initialized: %d mip count", static_cast<int>(mMipChain.size()));
	}
	
	void BloomPass::Shutdown()
	{
		for (Mip& mip : mMipChain)
			if (mip.renderTarget)
				delete mip.renderTarget;
		mMipChain.clear();
	}

	void BloomPass::BuildMipChain()
	{
		glm::uvec2 mipSize = mRenderResolution;
		for (unsigned int i = 0; i < mMipCount; i++)
		{
			mipSize = glm::max(mipSize / 2u, glm::uvec2(2));

			if (i < mMipChain.size())
			{
				mMipChain[i].size = mipSize;
				mMipChain[i].renderTarget->SetResolution(mipSize);
			}
			else
			{
				RenderTargetSpecification rtSpec;
				rtSpec.resolution = mipSize;
				rtSpec.colorAttachment = TextureFormat::R11G11B10F;

				mMipChain.push_back({ mipSize, new RenderTarget(rtSpec) });
			}
		}
	}

	void BloomPass::Render(unsigned int hdrTexture, unsigned int quadVAO, float bloomRadius)
	{
		Downsample(hdrTexture, quadVAO);
		Upsample(quadVAO, bloomRadius);
	}

	unsigned int BloomPass::GetBloomTextureId() const
	{
		if (mMipChain.size() < 1) return 0;
		return mMipChain[0].renderTarget->GetColorTexture()->GetId();
	}

	void BloomPass::SetRenderResolution(glm::uvec2 renderResolution)
	{
		renderResolution = glm::max(renderResolution, glm::uvec2(2, 2));
		if (mRenderResolution == renderResolution) 
			return;
		mRenderResolution = renderResolution;

		BuildMipChain();
	}

	void BloomPass::Downsample(unsigned int hdrTexture, unsigned int quadVAO)
	{
		mDownsampleShader->Bind();

		glDisable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		mDownsampleShader->SetInt("u_SrcTexture", 0);
		mDownsampleShader->SetVec2("u_SrcResolution", mRenderResolution);

		for (unsigned int i = 0; i < mMipChain.size(); i++)
		{
			mMipChain[i].renderTarget->Bind();
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// For the next iteration
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i].renderTarget->GetColorTexture()->GetId());
			mDownsampleShader->SetInt("u_SrcTexture", 0);
			mDownsampleShader->SetVec2("u_SrcResolution", mMipChain[i].size);
		}
	}

	void BloomPass::Upsample(unsigned int quadVAO, float bloomRadius)
	{
		mUpsampleShader->Bind();
		mUpsampleShader->SetFloat("u_BloomRadius", bloomRadius);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);

		for (int i = mMipChain.size() - 2; i >= 0; i--)
		{
			mMipChain[i].renderTarget->Bind();
			glViewport(0, 0, static_cast<GLsizei>(mMipChain[i].size.x), static_cast<GLsizei>(mMipChain[i].size.y));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mMipChain[i + 1].renderTarget->GetColorTexture()->GetId());
			mUpsampleShader->SetInt("u_SrcTexture", 0);
			mUpsampleShader->SetVec2("u_SrcResolution", mMipChain[i + 1].size);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}