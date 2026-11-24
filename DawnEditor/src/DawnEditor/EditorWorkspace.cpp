#include "EditorWorkspace.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Dawn/ImGui/OpenFontIcons.h>
#include <Dawn/Core/Application.h>

namespace Dawn
{
	void EditorWorkspace::Begin(EditorContext& ctx)
	{
		DrawMainMenuBar(ctx);
		DrawToolbar(ctx);
		DrawMainWorkspaceWindow();
	}

	void EditorWorkspace::End()
	{
		ImGui::End();
	}

	void EditorWorkspace::DrawMainMenuBar(EditorContext& ctx)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PushFont(NULL, 18);
			bool isFileMenuOpen = ImGui::BeginMenu("File");
			ImGui::PopFont();
			if (isFileMenuOpen)
			{
				ImGui::BeginDisabled(ctx.IsPlaying() || Application::Get()->GetScene() == nullptr);
				if (ImGui::MenuItem("Save Scene"))
					Application::Get()->SaveScene();
				ImGui::EndDisabled();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();
	}

	void EditorWorkspace::DrawToolbar(EditorContext& ctx)
	{
		ImGuiViewport* mainViewPort = ImGui::GetMainViewport();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
		if (ImGui::BeginViewportSideBar("Toolbar", mainViewPort, ImGuiDir_Up, mToolbarHeight, ImGuiWindowFlags_NoScrollbar))
		{
			float totalWidth = 2 * mPlayStopButtonWidth + ImGui::GetStyle().ItemSpacing.x;
			float cursorX = (ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f;

			ImGui::SetCursorPosX(cursorX);
			ImGui::SetCursorPosY((mToolbarHeight - mPlayStopButtonHeight) * 0.5f);

			ImGui::PushFont(NULL, 16.0f);

			bool isPlaying = ctx.IsPlaying();
			if (isPlaying)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.55f, 0.24f, 1.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1.0f);
			}
			ImGui::BeginDisabled(isPlaying);

			if (ImGui::Button(ICON_PLAY, ImVec2(mPlayStopButtonWidth, mPlayStopButtonHeight)))
				ctx.requestAction = EditorAction::Play;
			ImGui::EndDisabled();
			if (isPlaying)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();
			ImGui::BeginDisabled(!isPlaying);
			if (ImGui::Button(ICON_SQUARE, ImVec2(mPlayStopButtonWidth, mPlayStopButtonHeight)))
				ctx.requestAction = EditorAction::Stop;
			ImGui::EndDisabled();

			ImGui::PopFont();
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorWorkspace::DrawMainWorkspaceWindow()
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

		ImGui::Begin(mWindowName, NULL, windowFlags);
		ImGui::PopStyleVar(3);

		// --- DOCKSPACE ---
		ImGuiID dockSpaceId = ImGui::GetID(mDockspaceName);
		ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	}
}