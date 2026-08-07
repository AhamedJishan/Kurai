#include "Viewport.h"

#include <imgui/imgui.h>

namespace Dawn::Editor
{
	void BeginViewport()
	{
		ImGui::Begin("Viewport");
	}

	void EndViewport()
	{
		ImGui::End();
	}
}