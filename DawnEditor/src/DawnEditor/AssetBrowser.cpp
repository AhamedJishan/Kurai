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

	void DrawDirectoryEntryIcon(const std::filesystem::directory_entry& directoryEntry)
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

		ImGui::PushFont(NULL, sIconSize);
		ImGui::TextUnformatted(icon);
		ImGui::PopFont();
	}

	void DrawDirectoryContents(const std::filesystem::path& currentPath)
	{
		std::vector<std::filesystem::directory_entry> directoryEntries;
		for (auto& entry : std::filesystem::directory_iterator(currentPath))
			directoryEntries.push_back(entry);

		float availSizeX = ImGui::GetContentRegionAvail().x;
		int cellsPerRow = (availSizeX - 2.0f * sPadding) / sCellSize.x;
		if (cellsPerRow <= 0) cellsPerRow = 1;

		for (int i = 0; i < directoryEntries.size(); i++)
		{
			ImGui::PushID(i);

			std::string entryName = directoryEntries[i].path().filename().string();
			bool selected = entryName == sSelectedEntryName;

			if (ImGui::BeginChild("##CellWindow", sCellSize, true, ImGuiWindowFlags_NoScrollbar))
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
					// TODO: drag and drop logic
				}

				ImGui::SetCursorPos({ sPadding, sPadding });
				DrawDirectoryEntryIcon(directoryEntries[i]);

				ImGui::SetCursorPos(sTextPos);
				ImGui::TextUnformatted(entryName.c_str());
			}
			ImGui::EndChild();
			if ((i + 1) % cellsPerRow != 0)
				ImGui::SameLine();

			ImGui::PopID();
		}
	}


	void DrawAssetBrowser()
	{
		ImGui::Begin("Asset Browser");

		DrawDirectoryContents(sActiveDirectoryPath);

		ImGui::End();
	}
}