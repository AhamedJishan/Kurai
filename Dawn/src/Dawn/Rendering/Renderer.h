#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace Dawn
{
	// Forward declaration
	class RenderTarget;
	class BloomPass;
	class MeshRenderer;
	class Shader;
	class Texture;
	class RenderView;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Init(glm::vec2 resolution);

		void Draw();

		void AddRenderView(RenderView* renderView);
		void RemoveRenderView(RenderView* renderView);

		void AddMeshRenderer(MeshRenderer* meshRenderer);		// To be called by Constructor of MeshRenderer
		void RemoveMeshRenderer(MeshRenderer* meshRenderer);	// To be called by Destructor of MeshRenderer

	private:
		void DrawRenderView(const RenderView* renderView);
		void DrawScene(const glm::vec3& viewPosition, const glm::mat4& view, const glm::mat4& projection);
		void InitQuad();
		void DrawQuad();

	private:
		std::vector<RenderView*> mRenderViews;
		std::vector<MeshRenderer*> mMeshRenderers;

		unsigned int mQuadVAO = 0, mQuadVBO = 0;
		Shader* mPostProcessShader = nullptr;
	};
}