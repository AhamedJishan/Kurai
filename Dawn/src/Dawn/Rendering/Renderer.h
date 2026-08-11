#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Dawn
{
	// Forward declaration
	class RenderTarget;
	class BloomPass;
	class MeshRenderer;
	class Shader;
	class Texture;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Init(glm::vec2 resolution);

		void SetResolution(glm::vec2 resolution);
		glm::vec2 GetResolution() const { return mResolution; }

		void Draw();

		// To be called by Constructor of MeshRenderer
		void AddMeshRenderer(MeshRenderer* meshRenderer);
		// To be called by Destructor of MeshRenderer
		void RemoveMeshRenderer(MeshRenderer* meshRenderer);

	private:
		void DrawScene();
		void InitQuad();
		void DrawQuad();

	private:
		glm::vec2 mResolution = { 1920, 1080 };
		std::vector<MeshRenderer*> mMeshRenderers;
		BloomPass* mBloomPass = nullptr;

		RenderTarget* mHdrRenderTarget = nullptr;
		Texture* mHdrColorTexture = nullptr;
		Texture* mHdrDepthTexture = nullptr;

		unsigned int mQuadVAO = 0, mQuadVBO = 0;
		Shader* mPostProcessShader = nullptr;
	};
}