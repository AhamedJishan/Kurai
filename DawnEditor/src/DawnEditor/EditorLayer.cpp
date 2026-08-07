#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <Dawn/Utils/Log.h>
#include "DockSpace.h"

namespace Dawn
{
	void EditorLayer::OnAttach()
	{
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{

	}

	void EditorLayer::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		Editor::BeginDockSpace();
		
		Editor::EndDockSpace();
	}

}