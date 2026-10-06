#include "EnvironmentSettingsWindow.h"

#include <imgui/imgui.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>

namespace Dawn
{
	void EnvironmentSettingsWindow::Draw()
	{
		ImGui::Begin("Environment Settings");

		Scene* scene = Application::Get()->GetScene();
		if (!scene)
		{
			ImGui::End();
			return;
		}

		EnvironmentSettings& envSettings = scene->GetEnvironmentSettings();

		if (ImGui::BeginTable("Environment Settings properties", 2))
		{
			float availableWidth = ImGui::GetContentRegionAvail().x;
			ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 0.3f * availableWidth);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Bloom Radius");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			ImGui::DragFloat("##Bloom Radius", &envSettings.bloomRadius, 0.01f, 0.0f, 0.0f, "%.6g");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Bloom Strength");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			ImGui::DragFloat("##Bloom Strength", &envSettings.bloomStrength, 0.01f, 0.0f, 0.0f, "%.6g");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Fog Density");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			ImGui::DragFloat("##Fog Density", &envSettings.fogDensity, 0.01f, 0.0f, 0.0f, "%.6g");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Fog Color");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			ImGui::DragFloat3("##Fog Color", &envSettings.fogColor[0], 0.1f, 0.0f, 0.0f, "%.6g");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Ambient Color");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			ImGui::DragFloat3("##Ambient Color", &envSettings.ambientColor[0], 0.1f, 0.0f, 0.0f, "%.6g");

			ImGui::EndTable();
		}

		if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginTable("Directional Light properties", 2))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Intensity");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				ImGui::DragFloat("##Intensity", &envSettings.directionalLight.intensity, 0.01f, 0.0f, 0.0f, "%.6g");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Direction");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				ImGui::DragFloat3("##Direction", &envSettings.directionalLight.direction[0], 0.1f, 0.0f, 0.0f, "%.6g");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Color");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				ImGui::DragFloat3("##Color", &envSettings.directionalLight.color[0], 0.1f, 0.0f, 0.0f, "%.6g");

				ImGui::EndTable();
			}
		}

		ImGui::End();
	}
}