#include "EditorWorkspace.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Dawn/Core/Application.h>
#include <Dawn/ImGui/OpenFontIcons.h>

namespace Dawn::Editor
{
	namespace
	{
		const char* WORKSPACE_WINDOW_NAME = "Editor Workspace";
		const char* DOCKSPACE_NAME = "Main Dockspace";
		const float TOOLBAR_HEIGHT = 32.0f;
		const float PLAY_STOP_BUTTON_WIDTH = 30.0f;
		const float PLAY_STOP_BUTTON_HEIGHT = 24.0f;

		void DrawMainMenuBar()
		{
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
		}

		void DrawToolbar()
		{
			ImGuiViewport* mainViewPort = ImGui::GetMainViewport();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
			if (ImGui::BeginViewportSideBar("Toolbar", mainViewPort, ImGuiDir_Up, TOOLBAR_HEIGHT, ImGuiWindowFlags_NoScrollbar))
			{
				float totalWidth = 2 * PLAY_STOP_BUTTON_WIDTH + ImGui::GetStyle().ItemSpacing.x;
				float cursorX = (ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f;

				ImGui::SetCursorPosX(cursorX);
				ImGui::SetCursorPosY((TOOLBAR_HEIGHT - PLAY_STOP_BUTTON_HEIGHT) * 0.5f);

				ImGui::PushFont(NULL, 16.0f);

				bool isPlaying = Application::Get()->simulateScene;
				if (isPlaying)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.55f, 0.24f, 1.0f));
					ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1.0f);
				}
				ImGui::BeginDisabled(isPlaying);
				
				if (ImGui::Button(ICON_PLAY, ImVec2(PLAY_STOP_BUTTON_WIDTH, PLAY_STOP_BUTTON_HEIGHT)))
					Application::Get()->simulateScene = true;

				ImGui::EndDisabled();
				if (isPlaying)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
				}

				ImGui::SameLine();
				ImGui::BeginDisabled(!isPlaying);
				if (ImGui::Button(ICON_SQUARE, ImVec2(PLAY_STOP_BUTTON_WIDTH, PLAY_STOP_BUTTON_HEIGHT)))
				{
					Application::Get()->simulateScene = false;
				}
				ImGui::EndDisabled();

				ImGui::PopFont();
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}

		void DrawMainWorkspaceWindow()
		{
			ImGuiViewport* mainViewPort = ImGui::GetMainViewport();

			ImGui::SetNextWindowPos(mainViewPort->WorkPos);
			ImGui::SetNextWindowSize(mainViewPort->WorkSize);
			ImGui::SetNextWindowViewport(mainViewPort->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

			ImGui::Begin(WORKSPACE_WINDOW_NAME, NULL, windowFlags);
			ImGui::PopStyleVar(3);

			// --- DOCKSPACE ---
			ImGuiID dockSpaceId = ImGui::GetID(DOCKSPACE_NAME);
			ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
		}
	}

	// =====================================================================================
	void BeginEditorWorkspace()
	{
		DrawMainMenuBar();
		DrawToolbar();
		DrawMainWorkspaceWindow();
	}

	void EndEditorWorkspace()
	{
		ImGui::End();
	}
	// =====================================================================================
}