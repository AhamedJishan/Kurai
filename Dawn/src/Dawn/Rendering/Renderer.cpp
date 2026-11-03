#include "Renderer.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Dawn/Core/Application.h"
#include "Dawn/Asset/Assets.h"
#include "Dawn/Core/Window.h"
#include "Dawn/Core/Actor.h"
#include "Dawn/Core/Scene.h"
#include "Dawn/Core/Components/MeshRenderer.h"
#include <Dawn/Core/Components/Animator.h>
#include "RenderTarget.h"
#include "RenderView.h"
#include "BloomPass.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

namespace Dawn
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &mQuadVAO);
		glDeleteBuffers(1, &mQuadVBO);
	}
	
	bool Renderer::Init(glm::vec2 resolution)
	{
		InitQuad();
		mPostProcessShader = Assets::GetShader("post_process");

		return true;
	}
	
	void Renderer::Clear()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Render(const RenderView* renderView)
	{
		// --- HDR RENDER PASS ---
		const RenderTarget* hdrTarget = renderView->GetHdrRenderTarget();
		hdrTarget->Bind();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, hdrTarget->GetResolution().x, hdrTarget->GetResolution().y);
		DrawScene(renderView->GetViewPosition(), renderView->GetViewMatrix(), renderView->GetProjectionMatrix());

		// --- BLOOM PASS ---
		renderView->GetBloomPass()->Render(
			hdrTarget->GetColorTexture()->GetId(), 
			mQuadVAO, 
			Application::Get()->GetScene()->GetEnvironmentSettings().bloomRadius);

		// --- POST PROCESS QUAD TO SCREEN ---
		mPostProcessShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTarget->GetColorTexture()->GetId());
		mPostProcessShader->SetInt("u_HDRTexture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderView->GetBloomPass()->GetBloomTextureId());
		mPostProcessShader->SetInt("u_BloomTexture", 1);
		mPostProcessShader->SetFloat("u_BloomStrength", Application::Get()->GetScene()->GetEnvironmentSettings().bloomStrength);
		mPostProcessShader->SetVec2("u_SrcSize", glm::vec2(renderView->GetResolution()));

		if (renderView->RendersToScreen())
		{
			int x, y;
			Application::Get()->GetWindow()->GetFrameBufferSize(x, y);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, x, y);
			mPostProcessShader->SetVec2("u_DstSize", glm::vec2(x, y));
		}
		else
		{
			renderView->GetOutputRenderTarget()->Bind();
			glViewport(0, 0, renderView->GetResolution().x, renderView->GetResolution().y);
			mPostProcessShader->SetVec2("u_DstSize", glm::vec2(renderView->GetResolution().x, renderView->GetResolution().y));
		}

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		DrawQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::DrawScene(const glm::vec3& viewPosition, const glm::mat4& view, const glm::mat4& projection)
	{
		const EnvironmentSettings& environmentSettings = Application::Get()->GetScene()->GetEnvironmentSettings();

		for (MeshRenderer* meshRenderer : mMeshRenderers)
		{
			const std::vector<Mesh*>& meshes = meshRenderer->GetMeshes();
			const std::vector<Material*>& materials = meshRenderer->GetMaterials();
			for (Mesh* mesh : meshes)
			{
				glm::mat4 modelMatrix = meshRenderer->GetOwner()->GetTransform().ToMatrix();

				Material* mat = materials[mesh->GetMaterialIndex()];
				Animator* animator = meshRenderer->GetOwner()->GetComponent<Animator>();
				MeshType meshType = mesh->GetMeshType();

				bool skinned = (meshType == MeshType::Skinned) && (animator != nullptr);

				Shader* shader = Assets::GetShader(mat->GetName(), skinned);

				shader->Bind();
				mat->Apply(shader);
				mesh->Bind();

				shader->SetMat4("u_Model", modelMatrix);
				shader->SetMat4("u_View", view);
				shader->SetMat4("u_Projection", projection);

				if (skinned)
					shader->SetMat4s("u_MatrixPalette", animator->GetMatrixPalette());

				shader->SetFloat("u_FogDensity", environmentSettings.fogDensity);
				shader->SetVec3("u_FogColor", environmentSettings.fogColor);
				shader->SetVec3("u_ViewPosition", viewPosition);
				shader->SetVec3("u_AmbientColor", environmentSettings.ambientColor);
				shader->SetVec3("u_DirectionalLightColor", environmentSettings.directionalLight.color);
				shader->SetVec3("u_DirectionalLightDirection", environmentSettings.directionalLight.direction);
				shader->SetFloat("u_DirectionalLightIntensity", environmentSettings.directionalLight.intensity);

				glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, NULL);
			}
		}
	}

	void Renderer::InitQuad()
	{
		float vertices[] = {
			// positions        // texcoords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		glGenVertexArrays(1, &mQuadVAO);
		glGenBuffers(1, &mQuadVBO);
		glBindVertexArray(mQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	void Renderer::DrawQuad()
	{
		glBindVertexArray(mQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void Renderer::AddMeshRenderer(MeshRenderer* meshRenderer)
	{
		mMeshRenderers.push_back(meshRenderer);
	}
	
	void Renderer::RemoveMeshRenderer(MeshRenderer* meshRenderer)
	{
		auto it = std::find(mMeshRenderers.begin(), mMeshRenderers.end(), meshRenderer);
		if (it != mMeshRenderers.end())
			mMeshRenderers.erase(it);
	}
}