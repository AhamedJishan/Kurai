#include "AssetBrowser.h"

#include <vector>

namespace Dawn
{
	void AssetBrowser::Draw()
	{
		if (ImGui::Begin("Asset Browser"))
		{
			DrawDirectoryNavigator();
			DrawDirectoryContents(mActiveDirectoryPath);
		}
		ImGui::End();
	}

	void AssetBrowser::DrawDirectoryNavigator()
	{
		float availSizeX = ImGui::GetContentRegionAvail().x;

		if (ImGui::BeginChild("##DirectoryNavigator", { availSizeX, 24.0f }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			DrawDirBreadCrumbs();
		}
		ImGui::EndChild();
	}

	void AssetBrowser::DrawDirBreadCrumbs()
	{
		std::filesystem::path accumulatedPath;

		for (const auto& directory : mActiveDirectoryPath)
		{
			accumulatedPath /= directory;
			ImVec2 textSize = ImGui::CalcTextSize(directory.string().c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosY(2);
			if (ImGui::Selectable(directory.string().c_str(), false, 0, textSize))
			{
				mActiveDirectoryPath = accumulatedPath;
				return;
			}

			ImGui::SameLine();
			ImGui::SetCursorPosY(2);
			ImGui::TextUnformatted("/");
		}
	}

	void AssetBrowser::DrawDirectoryContents(const std::filesystem::path& currentPath)
	{
		std::vector<std::filesystem::directory_entry> directoryEntries;
		for (auto& entry : std::filesystem::directory_iterator(currentPath))
			directoryEntries.push_back(entry);

		if (ImGui::BeginChild("##CellContainerWindow", ImGui::GetContentRegionAvail(), true))
		{
			ImVec2 availSize = ImGui::GetContentRegionAvail();
			int cellsPerRow = (availSize.x - 2.0f * mPadding) / mCellSize.x;
			if (cellsPerRow <= 0) cellsPerRow = 1;

			for (int i = 0; i < directoryEntries.size(); i++)
			{
				ImGui::PushID(i);

				std::string entryName = directoryEntries[i].path().filename().string();
				bool selected = entryName == mSelectedEntryName;

				if (ImGui::BeginChild("##CellWindow", mCellSize, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
				{
					ImGui::SetCursorPos({});
					if (ImGui::Selectable("##CellSelectable", selected, ImGuiSelectableFlags_AllowDoubleClick, mCellSize))
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && directoryEntries[i].is_directory())
						{
							mActiveDirectoryPath = directoryEntries[i].path();
						}
						else
						{
							mSelectedEntryName = entryName;
						}
					}

					if (ImGui::BeginDragDropSource())
					{
						std::string path = directoryEntries[i].path().string();
						ImGui::SetDragDropPayload("ASSET_PATH", path.c_str(), path.size() + 1);

						ImGui::TextUnformatted(entryName.c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::SetCursorPos({ mPadding, mPadding });
					ImGui::PushFont(NULL, mIconSize);
					ImGui::TextUnformatted(GetDirectoryEntryIcon(directoryEntries[i]));
					ImGui::PopFont();

					ImGui::SetCursorPos(mTextPos);
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

	const char* AssetBrowser::GetDirectoryEntryIcon(const std::filesystem::directory_entry& directoryEntry)
	{
		const char* icon;
		if (directoryEntry.is_directory())
		{
			icon = ICON_FOLDER;
		}
		else
		{
			std::string extension = directoryEntry.path().extension().string();
			auto it = mFileExtToIconMap.find(extension);
			if (it != mFileExtToIconMap.end())
				icon = it->second;
			else
				icon = ICON_FILE;
		}

		return icon;
	}
}