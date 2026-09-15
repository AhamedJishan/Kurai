#include "AssetBrowser.h"

#include <imgui/imgui.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <Dawn/ImGui/OpenFontIcons.h>

namespace Dawn::Editor
{
	static std::string sSelectedEntryName = "";
	static std::filesystem::path sActiveDirectoryPath = "Assets";

	static const ImVec2 sCellSize = { 100.0f, 110.0f };
	static const float sPadding = 10.0f;
	static const float sIconSize = sCellSize.x - 2.0f * sPadding;
	static const ImVec2 sTextPos = { sPadding, sIconSize };

	static const std::unordered_map<std::string, const char*> sFileExtToIconMap =
	{
		{".frag", ICON_FILE_TEXT}, {".vert", ICON_FILE_TEXT}, {".scene", ICON_FILE_TEXT}, {".mtl", ICON_FILE_TEXT},
		{".png", ICON_IMAGE}, {".jpg", ICON_IMAGE}, {".jpeg", ICON_IMAGE}, {".tif", ICON_IMAGE},
		{".bank", ICON_MUSIC},
		{".ttf", ICON_ITALIC},
		{".dae", ICON_BOX}, {".obj", ICON_BOX}, {".fbx", ICON_BOX}, {".blend", ICON_BOX}
	};


	const char* GetDirectoryEntryIcon(const std::filesystem::directory_entry& directoryEntry)
	{
		const char* icon;
		if (directoryEntry.is_directory())
		{
			icon = ICON_FOLDER;
		}
		else
		{
			std::string extension = directoryEntry.path().extension().string();
			auto it = sFileExtToIconMap.find(extension);
			if (it != sFileExtToIconMap.end())
				icon = it->second;
			else
				icon = ICON_FILE;
		}

		return icon;
	}

	void DrawDirectoryContents(const std::filesystem::path& currentPath)
	{
		std::vector<std::filesystem::directory_entry> directoryEntries;
		for (auto& entry : std::filesystem::directory_iterator(currentPath))
			directoryEntries.push_back(entry);

		ImVec2 availSize = ImGui::GetContentRegionAvail();
		int cellsPerRow = (availSize.x - 2.0f * sPadding) / sCellSize.x;
		if (cellsPerRow <= 0) cellsPerRow = 1;
		
		if (ImGui::BeginChild("##CellContainerWindow", availSize, true))
		{
			for (int i = 0; i < directoryEntries.size(); i++)
			{
				ImGui::PushID(i);

				std::string entryName = directoryEntries[i].path().filename().string();
				bool selected = entryName == sSelectedEntryName;

				if (ImGui::BeginChild("##CellWindow", sCellSize, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
				{
					ImGui::SetCursorPos({});
					if (ImGui::Selectable("##CellSelectable", selected, ImGuiSelectableFlags_AllowDoubleClick, sCellSize))
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && directoryEntries[i].is_directory())
						{
							sActiveDirectoryPath = directoryEntries[i].path();
						}
						else
						{
							sSelectedEntryName = entryName;
						}
					}

					if (ImGui::BeginDragDropSource())
					{
						std::string path = directoryEntries[i].path().string();
						ImGui::SetDragDropPayload("ASSET_PATH", path.c_str(), path.size() + 1);

						ImGui::TextUnformatted(entryName.c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::SetCursorPos({ sPadding, sPadding });
					ImGui::PushFont(NULL, sIconSize);
					ImGui::TextUnformatted(GetDirectoryEntryIcon(directoryEntries[i]));
					ImGui::PopFont();

					ImGui::SetCursorPos(sTextPos);
					ImGui::TextUnformatted(entryName.c_str());
					if (ImGui::IsItemHovered())
						ImGui::SetItemTooltip(entryName.c_str());
				}
				ImGui::EndChild();
				if ((i + 1) % cellsPerRow != 0)
					ImGui::SameLine();

				ImGui::PopID();
			}
		}
		ImGui::EndChild();
	}

	void DrawDirBreadCrumbs()
	{
		std::filesystem::path accumulatedPath;

		for (const auto& directory : sActiveDirectoryPath)
		{
			accumulatedPath /= directory;
			ImVec2 textSize = ImGui::CalcTextSize(directory.string().c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosY(2);
			if (ImGui::Selectable(directory.string().c_str(), false, 0, textSize))
			{
				sActiveDirectoryPath = accumulatedPath;
				return;
			}

			ImGui::SameLine();
			ImGui::SetCursorPosY(2);
			ImGui::TextUnformatted("/");
		}
	}

	void DrawDirectoryNavigator()
	{
		float availSizeX = ImGui::GetContentRegionAvail().x;

		if (ImGui::BeginChild("##DirectoryNavigator", {availSizeX, 24.0f}, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			DrawDirBreadCrumbs();
		}
		ImGui::EndChild();
	}


	void DrawAssetBrowser()
	{
		if (ImGui::Begin("Asset Browser"))
		{
			DrawDirectoryNavigator();
			DrawDirectoryContents(sActiveDirectoryPath);
		}
		ImGui::End();
	}
}