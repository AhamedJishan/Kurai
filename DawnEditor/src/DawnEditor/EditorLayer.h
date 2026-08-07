#pragma once
#include <Dawn/Core/Layer.h>

namespace Dawn
{
	// Forward declarations
	class Actor;

	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		Actor* mSelectedActor = nullptr;
	};
}