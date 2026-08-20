#include "Inspector.h"

#include <imgui/imgui.h>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Transform.h>

namespace Dawn::Editor
{
	void DrawTransform(Actor* actor)
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginTable("Transform", 2))
			{
				float availableWidth = ImGui::GetContentRegionAvail().x;
				ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 0.3f * availableWidth);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				Transform& transform = actor->GetTransform();
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				ImGui::DragFloat3("##Position", &transform.Position[0], .1f, 0.0f, 0.0f, "%.2f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Scale");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				ImGui::DragFloat3("##Scale", &transform.Scale[0], .10f, 0.0f, 0.0f, "%.2f");

				glm::vec3 rotEuler = glm::degrees(glm::eulerAngles(transform.Rotation));

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat3("##Rotation", &rotEuler[0], .10f, 0.0f, 0.0f, "%.2f"))
					transform.Rotation = glm::quat(glm::radians(rotEuler));
					
				ImGui::EndTable();
			}
		}
	}



	void BeginInspector(Actor* selectedActor)
	{
		ImGui::Begin("Inspector");

		if (!selectedActor)
			return;

		// Display Transform
		DrawTransform(selectedActor);

		// Display each property

		// Add Component
	}

	void EndInspector()
	{
		ImGui::End();
	}
}