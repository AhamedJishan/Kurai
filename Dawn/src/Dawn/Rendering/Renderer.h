#pragma once

#include <vector>
#include <glm/vec3.hpp>

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

		bool Init();

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
		std::vector<MeshRenderer*> mMeshRenderers;
		//HDRFramebuffer* mHDRFrameBuffer = nullptr;
		BloomPass* mBloomPass = nullptr;

		RenderTarget* mHdrRenderTarget = nullptr;
		Texture* mHdrColorTexture = nullptr;
		Texture* mHdrDepthTexture = nullptr;

		unsigned int mQuadVAO = 0, mQuadVBO = 0;
		Shader* mPostProcessShader = nullptr;
	};
}