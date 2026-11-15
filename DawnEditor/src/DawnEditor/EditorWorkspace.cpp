#include "EditorWorkspace.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Dawn/Core/Application.h>
#include <Dawn/ImGui/Fonts.h>
#include <Dawn/ImGui/OpenFontIcons.h>

namespace Dawn::Editor
{
	static const char* WINDOW_TITLE = "Editor Workspace";
	static const char* DOCKSPACE_NAME = "Main Dockspace";
	static const float TOOLBAR_HEIGHT = 34.0f;
	static const float PLAY_STOP_BUTTON_SIZE = 26.0f;

	void BeginEditorWorkspace()
	{
		ImGuiViewport* mainViewPort = ImGui::GetMainViewport();

		// --- MENU BAR ---
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PushFont(NULL, 18);
			bool isFileMenuOpen = ImGui::BeginMenu("File");
			ImGui::PopFont();
			if (isFileMenuOpen)
			{
				if (ImGui::MenuItem("Save Scene"))
					Application::Get()->SaveScene();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();

		// --- TOOL BAR ---
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
		if (ImGui::BeginViewportSideBar("Toolbar", mainViewPort, ImGuiDir_Up, TOOLBAR_HEIGHT, ImGuiWindowFlags_NoScrollbar))
		{
			float totalWidth = 2 * PLAY_STOP_BUTTON_SIZE + ImGui::GetStyle().ItemSpacing.x;
			float cursorX = (ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f;

			ImGui::SetCursorPosX(cursorX);
			ImGui::SetCursorPosY((TOOLBAR_HEIGHT - PLAY_STOP_BUTTON_SIZE) * 0.5f);

			if (ImGui::Button(ICON_PLAY, ImVec2(PLAY_STOP_BUTTON_SIZE, PLAY_STOP_BUTTON_SIZE))) { /* Play Logic */ }
			ImGui::SameLine();
			if (ImGui::Button(ICON_SQUARE, ImVec2(PLAY_STOP_BUTTON_SIZE, PLAY_STOP_BUTTON_SIZE))) { /* Stop Logic */ }
		}
		ImGui::End();
		ImGui::PopStyleVar();

		// --- WINDOW ---
		ImGui::SetNextWindowPos(mainViewPort->WorkPos);
		ImGui::SetNextWindowSize(mainViewPort->WorkSize);
		ImGui::SetNextWindowViewport(mainViewPort->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		ImGui::Begin(WINDOW_TITLE, NULL, windowFlags);
		ImGui::PopStyleVar(3);

		// --- DOCKSPACE ---
		ImGuiID dockSpaceId = ImGui::GetID(DOCKSPACE_NAME);
		ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void EndEditorWorkspace()
	{
		ImGui::End();
	}
}