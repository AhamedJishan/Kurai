#include "Renderer.h"
#include "Utils/Log.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Core/Application.h"
#include "Asset/Assets.h"
#include "Core/Window.h"
#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Core/Components/Camera.h"
#include "Core/Components/MeshRenderer.h"
#include <Core/Components/Animator.h>
#include "HDRFramebuffer.h"
#include "BloomPass.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"

namespace Dawn
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &mQuadVAO);
		glDeleteBuffers(1, &mQuadVBO);

		if (mHDRFrameBuffer) delete mHDRFrameBuffer;
	}
	
	bool Renderer::Init()
	{
		// Assumes a valid Opengl context is already initialised
		// which was done in Window::Init()
		int x, y;
		Application::Get()->GetWindow()->GetFrameBufferSize(x, y);
		glViewport(0, 0, x, y);

		Application::Get()->GetWindow()->SetFrameBufferSizeCallback([this](int width, int height) { glViewport(0, 0, width, height);});

		mHDRFrameBuffer = new HDRFramebuffer();
		mBloomPass = new BloomPass(6);

		InitQuad();
		mPostProcessShader = Assets::GetShader("post_process");

		return true;
	}
	
	void Renderer::Draw()
	{
		// --- HDR FRAMEBUFFER RENDER PASS --- 
		mHDRFrameBuffer->BindFrameBuffer();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		DrawScene();

		// --- BLOOM PASS ---
		mBloomPass->Render(mHDRFrameBuffer->GetHDRTextureId(), mQuadVAO, 2.0f);

		// --- POST PROCESS QUAD TO SCREEN ---
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		mPostProcessShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mHDRFrameBuffer->GetHDRTextureId());
		mPostProcessShader->SetInt("u_HDRTexture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mBloomPass->GetBloomTextureId());
		mPostProcessShader->SetInt("u_BloomTexture", 1);
		mPostProcessShader->SetFloat("u_BloomStrength", Application::Get()->GetScene()->GetEnvironmentSettings().bloomStrength);

		DrawQuad();
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

	void Renderer::DrawScene()
	{
		Camera* cam = Application::Get()->GetScene()->GetActiveCamera();
		if (!cam)
			return;

		glm::mat4 viewMatrix = cam->GetView();
		glm::mat4 projectionMatrix = cam->GetProjection();

		const EnvironmentSettings& environmentSettings = Application::Get()->GetScene()->GetEnvironmentSettings();

		for (MeshRenderer* meshRenderer : mMeshRenderers)
		{
			const std::vector<Mesh*>& meshes = meshRenderer->GetMeshes();
			const std::vector<Material*>& materials = meshRenderer->GetMaterials();
			for (Mesh* mesh : meshes)
			{
				glm::mat4 modelMatrix = meshRenderer->GetOwner()->GetTransform().ToMatrix();

				Material* mat = materials[mesh->GetMaterialIndex()];
				MeshType meshType = mesh->GetMeshType();

				Shader* shader = Assets::GetShader(mat->GetName(), meshType == MeshType::Skinned);

				shader->Bind();
				mat->Apply(shader);
				mesh->Bind();

				shader->SetMat4("u_Model", modelMatrix);
				shader->SetMat4("u_View", viewMatrix);
				shader->SetMat4("u_Projection", projectionMatrix);

				if (meshType == MeshType::Skinned)
				{
					Animator* animator = meshRenderer->GetOwner()->GetComponent<Animator>();
					shader->SetMat4s("u_MatrixPalette", animator->GetMatrixPalette());
				}

				shader->SetFloat("u_FogDensity", environmentSettings.fogDensity);
				shader->SetVec3("u_FogColor", environmentSettings.fogColor);
				shader->SetVec3("u_CameraPosition", cam->GetOwner()->GetTransform().Position);
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
}