#include "Hierarchy.h"

#include <imgui/imgui.h>
#include <string>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>

namespace Dawn
{
	void Hierarchy::Draw(Actor*& selectedActor)
	{
		ImGui::Begin("Hierarchy");

		Scene* scene = Application::Get()->GetScene();
		if (!scene)
			return;

		for (Actor* actor : scene->GetActors())
		{
			if (actor == mActorBeingRenamed)
			{
				ImGui::SetNextItemWidth(-1);

				if (mFocusActorRename)
				{
					ImGui::SetKeyboardFocusHere();
					mFocusActorRename = false;
				}

				if (ImGui::InputText("##ActorRename", mActorRenameBuffer, sizeof(mActorRenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					mActorBeingRenamed->SetName(mActorRenameBuffer);
					mActorBeingRenamed = nullptr;
				}

				if (ImGui::IsItemDeactivated())
					mActorBeingRenamed = nullptr;

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
					mActorBeingRenamed = actor;
					mFocusActorRename = true;
					strcpy(mActorRenameBuffer, actor->GetName().c_str());
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