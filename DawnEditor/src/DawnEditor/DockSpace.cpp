#include "DockSpace.h"

#include <imgui/imgui.h>

namespace Dawn::Editor
{
	void BeginDockSpace()
	{
		const ImGuiViewport* mainViewPort = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(mainViewPort->WorkPos);
		ImGui::SetNextWindowSize(mainViewPort->WorkSize);
		ImGui::SetNextWindowViewport(mainViewPort->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiWindowFlags dockSpaceFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		ImGui::Begin("EditorDockSpace", NULL, dockSpaceFlags);
		ImGui::PopStyleVar(3);

		ImGuiID dockSpaceId = ImGui::GetID("EditorDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New Scene");
				ImGui::MenuItem("Open Scene");
				ImGui::MenuItem("Save Scene");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void EndDockSpace()
	{
		ImGui::End();
	}
}