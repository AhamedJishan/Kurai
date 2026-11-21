#include "EditorWorkspace.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Dawn/Utils/Log.h>
#include <Dawn/ImGui/OpenFontIcons.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Core/SceneSerializer.h>

namespace Dawn
{
	void EditorWorkspace::Begin()
	{
		DrawMainMenuBar();
		DrawToolbar();
		DrawMainWorkspaceWindow();
	}

	void EditorWorkspace::End()
	{
		ImGui::End();
	}

	void EditorWorkspace::DrawMainMenuBar()
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

	void EditorWorkspace::DrawToolbar()
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

			bool isPlaying = Application::Get()->simulateScene;
			if (isPlaying)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.55f, 0.24f, 1.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1.0f);
			}
			ImGui::BeginDisabled(isPlaying);

			if (ImGui::Button(ICON_PLAY, ImVec2(mPlayStopButtonWidth, mPlayStopButtonHeight)))
			{
				Scene* activeScene = Application::Get()->GetScene();
				if (activeScene)
				{
					Application::Get()->simulateScene = true;
					mPrePlaySceneWasDirty = activeScene->IsDirty();
					SceneSerializer::Serialize(activeScene, mPrePlaySceneSnapshot);
				}
				else
				{
					LOG_WARN("No active Scene to play!");
				}
			}

			ImGui::EndDisabled();
			if (isPlaying)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();
			ImGui::BeginDisabled(!isPlaying);
			if (ImGui::Button(ICON_SQUARE, ImVec2(mPlayStopButtonWidth, mPlayStopButtonHeight)))
			{
				Application::Get()->simulateScene = false;
				Scene* deserializedScene = SceneSerializer::Deserialize(mPrePlaySceneSnapshot);
				if (deserializedScene)
				{
					if (mPrePlaySceneWasDirty)
						deserializedScene->MarkDirty();
					Application::Get()->LoadScene(deserializedScene, Application::Get()->GetScenePath());
					mPrePlaySceneSnapshot.clear();
				}
				else
					LOG_ERROR("Failed to restore scene after Editor Play");
			}
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