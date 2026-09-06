#include "AssetBrowser.h"

#include <imgui/imgui.h>
#include <filesystem>
#include <vector>
#include <string>
#include <Dawn/ImGui/OpenFontIcons.h>

namespace Dawn::Editor
{
	static std::string sSelectedFilename = "";

	void DrawAssetBrwoser()
	{
		ImGui::Begin("Asset Browser");

		std::filesystem::path root = "Assets";

		std::vector<std::filesystem::path> files;
		for (auto& entry : std::filesystem::directory_iterator(root))
			files.push_back(entry.path());

		const ImVec2 cellSize = { 100.0f, 120.0f };
		const float padding = 10.0f;
		const float iconSize = cellSize.x - 2.0f * padding;
		const ImVec2 textPos = {padding, padding + iconSize};
		float availSizeX = ImGui::GetContentRegionAvail().x;
		int cellsPerRow = (availSizeX - 2.0f * padding) / cellSize.x;
		if (cellsPerRow <= 0) cellsPerRow = 1;

		for (int i = 0; i < files.size(); i++)
		{
			ImGui::PushID(i);

			std::string filename = files[i].filename().string();
			bool selected = filename == sSelectedFilename;

			if (ImGui::BeginChild("##CellWindow", cellSize, true, ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::SetCursorPos({});
				if (ImGui::Selectable("##CellSelectable", selected, ImGuiSelectableFlags_AllowDoubleClick, cellSize))
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						// TODO: open the file
					}
					else
					{
						sSelectedFilename = filename;
					}
				}

				if (ImGui::BeginDragDropSource())
				{
					// TODO: drag and drop logic
				}

				ImGui::SetCursorPos({ padding, padding });
				ImGui::PushFont(NULL, iconSize);
				ImGui::TextUnformatted(ICON_FOLDER);
				ImGui::PopFont();

				ImGui::SetCursorPos(textPos);
				ImGui::TextUnformatted(filename.c_str());
			}
			ImGui::EndChild();
			if ((i + 1) % cellsPerRow != 0)
				ImGui::SameLine();

			ImGui::PopID();
		}

		ImGui::End();
	}
}