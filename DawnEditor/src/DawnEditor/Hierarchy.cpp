#include "Hierarchy.h"

#include <imgui/imgui.h>
#include <string>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>

namespace Dawn::Editor
{
	static Actor* sActorBeingRenamed = nullptr;
	static char sActorRenameBuffer[256] = "";
	static bool sFocusActorRename = false;

	void DrawHierarchy(Actor*& selectedActor)
	{
		ImGui::Begin("Hierarchy");

		Scene* scene = Application::Get()->GetScene();
		if (!scene)
			return;

		for (Actor* actor : scene->GetActors())
		{
			if (actor == sActorBeingRenamed)
			{
				ImGui::SetNextItemWidth(-1);

				if (sFocusActorRename)
				{
					ImGui::SetKeyboardFocusHere();
					sFocusActorRename = false;
				}

				if (ImGui::InputText("##ActorRename", sActorRenameBuffer, sizeof(sActorRenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					sActorBeingRenamed->SetName(sActorRenameBuffer);
					sActorBeingRenamed = nullptr;
				}

				if (ImGui::IsItemDeactivated())
					sActorBeingRenamed = nullptr;

				continue; // NEXT ITERATION
			}

			ImGui::PushID((void*)actor);

			bool selected = actor == selectedActor;
			if (ImGui::Selectable(actor->GetName().c_str(), selected, ImGuiSelectableFlags_SpanAllColumns))
				selectedActor = actor;

			ImGui::PopID();

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Rename"))
				{
					sActorBeingRenamed = actor;
					sFocusActorRename = true;
					strcpy(sActorRenameBuffer, actor->GetName().c_str());
				}
				if (ImGui::MenuItem("Delete"))
				{
					scene->DestroyActor(actor);
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::BeginPopupContextWindow("Hierarchy Context Window", ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Actor"))
			{
				scene->CreateActor();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}