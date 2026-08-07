#include "EditorLayer.h"

#include <imgui/imgui.h>
#include "Theme.h"
#include "DockSpace.h"
#include "Hierarchy.h"
#include "Viewport.h"
#include "Inspector.h"

namespace Dawn
{
	void EditorLayer::OnAttach() {}
	void EditorLayer::OnDetach() {}
	void EditorLayer::OnUpdate(float deltaTime) {}

	void EditorLayer::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		Editor::ApplyTheme();

		Editor::BeginDockSpace();

		Editor::BeginHierarchy();
		Editor::EndHierarchy();

		Editor::BeginViewport();
		Editor::EndViewport();

		Editor::BeginInspector();
		Editor::EndInspector();
		
		Editor::EndDockSpace();

		Editor::ResetTheme();
	}

}