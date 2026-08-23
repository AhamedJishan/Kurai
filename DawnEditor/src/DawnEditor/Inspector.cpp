#include "Inspector.h"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/ComponentFactory.h>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Component.h>
#include <Dawn/Core/Property.h>
#include <Dawn/Core/Transform.h>

namespace Dawn::Editor
{
	bool DrawQuatInputField(const char* label, glm::quat& value, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.2f")
	{
		ImGuiStorage* imGuiStorage = ImGui::GetStateStorage();
		ImGuiID idx = ImGui::GetID((std::string(label) + "_x").c_str());
		ImGuiID idy = ImGui::GetID((std::string(label) + "_y").c_str());
		ImGuiID idz = ImGui::GetID((std::string(label) + "_z").c_str());

		glm::vec3 rotEuler;
		rotEuler.x = imGuiStorage->GetFloat(idx);
		rotEuler.y = imGuiStorage->GetFloat(idy);
		rotEuler.z = imGuiStorage->GetFloat(idz);

		glm::quat reconstructed = glm::quat(glm::radians(rotEuler));
		if (std::abs(glm::dot(reconstructed, value)) < 0.9999f)
			rotEuler = glm::degrees(glm::eulerAngles(value));

		bool edited = ImGui::DragFloat3("##Rotation", &rotEuler[0], speed, min, max, format);

		if (edited)
			value = glm::quat(glm::radians(rotEuler));

		imGuiStorage->SetFloat(idx, rotEuler.x);
		imGuiStorage->SetFloat(idy, rotEuler.y);
		imGuiStorage->SetFloat(idz, rotEuler.z);

		return edited;
	}

	bool DrawStringInputField(const char* label, std::string* value)
	{
		ImGui::InputText(label, value, ImGuiInputTextFlags_EnterReturnsTrue);
		return ImGui::IsItemDeactivatedAfterEdit();
	}

	bool DrawStringListInputField(const char* label, std::vector<std::string>* value)
	{
		int idxToBeRemoved = -1;
		bool edited = false;

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();

		ImGui::PushID(label);

		for (size_t i = 0; i < value->size(); i++)
		{
			ImGui::PushID(static_cast<int>(i));

			ImGui::SetNextItemWidth(-30);
			ImGui::InputText(label, &(*value)[i], ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::IsItemDeactivatedAfterEdit())
				edited = true;

			ImGui::SameLine();
			if (ImGui::Button(" - "))
				idxToBeRemoved = static_cast<int>(i);

			ImGui::PopID();
		}

		if (ImGui::Button("+ Add Item"))
			value->push_back("");

		ImGui::PopID();

		if (idxToBeRemoved >= 0 && idxToBeRemoved < value->size())
		{
			value->erase(value->begin() + idxToBeRemoved);
			edited = true;
		}

		return edited;
	}

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

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				DrawQuatInputField("##Rotation", transform.Rotation);
					
				ImGui::EndTable();
			}
		}
	}

	// returns bool, whether the property was changed or not
	bool DrawProperty(Property property)
	{
		std::string propertyLabel("##" + property.name);

		ImGui::TableNextColumn(); 
		ImGui::Text(property.name.c_str());

		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1);

		bool edited = false;

		switch (property.type)
		{
		case PropertyType::Int: edited = ImGui::DragInt(propertyLabel.c_str(), static_cast<int*>(property.data), 0.1f); break;
		case PropertyType::Bool: edited = ImGui::Checkbox(propertyLabel.c_str(), static_cast<bool*>(property.data)); break;
		case PropertyType::Float: edited = ImGui::DragFloat(propertyLabel.c_str(), static_cast<float*>(property.data), 0.1f); break;
		case PropertyType::Vec2: edited = ImGui::DragFloat2(propertyLabel.c_str(), glm::value_ptr(*static_cast<glm::vec2*>(property.data)), 0.1f); break;
		case PropertyType::Vec3: edited = ImGui::DragFloat3(propertyLabel.c_str(), glm::value_ptr(*static_cast<glm::vec3*>(property.data)), 0.1f); break;
		case PropertyType::Vec4: edited = ImGui::DragFloat4(propertyLabel.c_str(), glm::value_ptr(*static_cast<glm::vec4*>(property.data)), 0.1f); break;
		case PropertyType::Quat: edited = DrawQuatInputField(propertyLabel.c_str(), *static_cast<glm::quat*>(property.data)); break;
		case PropertyType::String: edited = DrawStringInputField(propertyLabel.c_str(), static_cast<std::string*>(property.data)); break;
		case PropertyType::StringList: edited = DrawStringListInputField(propertyLabel.c_str(), static_cast<std::vector<std::string>*>(property.data)); break;
		// TODO: StringPairList
		default: break;
		}

		return edited;			
	}

	void DrawComponent(Component* component)
	{
		std::string componentName = Application::Get()->GetComponentFactory()->GetComponentName(component);
		std::vector<Property> properties = component->GetProperties();

		if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginTable(componentName.c_str(), 2))
			{
				float availableWidth = ImGui::GetContentRegionAvail().x;
				ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 0.3f * availableWidth);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				for (Property& property : properties)
				{
					ImGui::TableNextRow();
					if (DrawProperty(property))
						component->OnPropertiesChanged();
				}
				ImGui::EndTable();
			}
		}
	}



	void BeginInspector(Actor* selectedActor)
	{
		ImGui::Begin("Inspector");

		if (!selectedActor)
			return;

		DrawTransform(selectedActor);

		for (Component* component : selectedActor->GetComponents())
			DrawComponent(component);

		// Add Component
	}

	void EndInspector()
	{
		ImGui::End();
	}
}