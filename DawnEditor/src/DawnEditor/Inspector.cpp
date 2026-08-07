#include "Inspector.h"

#include <imgui/imgui.h>

namespace Dawn::Editor
{
	void BeginInspector()
	{
		ImGui::Begin("Inspector");
	}

	void EndInspector()
	{
		ImGui::End();
	}
}