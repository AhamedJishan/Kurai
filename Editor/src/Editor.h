#pragma once
#include <Core/Layer.h>

namespace Dawn
{
	class Editor : public Layer
	{
		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	};
}