#pragma once
#include <Dawn/Core/Layer.h>

namespace Dawn
{
	// Forward declarations
	class Actor;
	class RenderTarget;
	class Texture;

	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		Actor* mSelectedActor = nullptr;
		RenderTarget* mViewportRT = nullptr;
		Texture* mViewportTexture = nullptr;
	};
}