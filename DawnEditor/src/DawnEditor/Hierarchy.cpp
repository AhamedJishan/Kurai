#include "Hierarchy.h"

#include <imgui/imgui.h>
#include <vector>
#include <string>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>

namespace Dawn::Editor
{
	static Actor* sActorBeingRenamed = nullptr;
	static char sActorRenameBuffer[256] = "";
	static bool sFocusActorRename = false;

	void BeginHierarchy(Actor*& selectedActor)
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

			bool selected = actor == selectedActor;
			if (ImGui::Selectable(actor->GetName().c_str(), selected, ImGuiSelectableFlags_SpanAllColumns))
				selectedActor = actor;

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
					Application::Get()->GetScene()->DestroyActor(actor);
				}
				ImGui::EndPopup();
			}
		}
	}

	void EndHierarchy()
	{
		ImGui::End();
	}
}