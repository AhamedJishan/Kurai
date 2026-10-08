#include "Hierarchy.h"

#include <imgui/imgui.h>
#include <string>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/ImGui/Fonts.h>

namespace Dawn
{
	void Hierarchy::Draw(Actor*& selectedActor)
	{
		ImGui::Begin("Hierarchy");

		Scene* scene = Application::Get()->GetScene();
		if (!scene)
		{
			ImGui::End();
			return;
		}

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.18f, 1.0f));
		if (ImGui::BeginChild("Scene Title Window", ImVec2(0, 24), 0, ImGuiWindowFlags_NoScrollbar))
		{
			ScopedFont sf(Fonts::Bold, 18.0f);
			float availY = ImGui::GetContentRegionAvail().y;
			float textHeight = ImGui::CalcTextSize("DUMMY TEXT").y;
			ImGui::SetCursorPos(ImVec2(6.0f, (availY - textHeight) / 2.0f - 1.0f));	// -1.0f small adjustment
			
			std::string sceneDisplayText = scene->GetName();
			if (scene->IsDirty())
				sceneDisplayText += " *";
			ImGui::Text("%s", sceneDisplayText.c_str());
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();


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
					scene->MarkDirty();
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
					scene->MarkDirty();
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::BeginPopupContextWindow("Hierarchy Context Window", ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Actor"))
			{
				scene->CreateActor();
				scene->MarkDirty();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}