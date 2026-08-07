#include "Hierarchy.h"

#include <imgui/imgui.h>

namespace Dawn::Editor
{
	void BeginHierarchy()
	{
		ImGui::Begin("Hierarchy");
	}

	void EndHierarchy()
	{
		ImGui::End();
	}
}