#include "AssetBrowser.h"

#include <vector>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Utils/FileSystem.h>
#include <Dawn/Utils/Log.h>

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
					if (ImGui::Selectable("##CellSelectable", selected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap, mCellSize))
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (directoryEntries[i].is_directory())
								mActiveDirectoryPath = directoryEntries[i].path();
							else if (directoryEntries[i].path().extension().string() == Scene::GetExtension())
								Application::Get()->LoadScene(directoryEntries[i].path().string());
						}
						else
						{
							mSelectedEntryName = entryName;
						}
					}
					if (directoryEntries[i].is_regular_file() && ImGui::BeginPopupContextWindow("File Context Window"))
					{
						if (ImGui::MenuItem("Rename"))
						{
							mFocusFileRename = true;
							mRenamingFilename = directoryEntries[i].path().filename().string();
							strcpy_s(mRenameFileBuffer, directoryEntries[i].path().stem().string().c_str());
						}
						ImGui::EndPopup();
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
					if (mRenamingFilename == directoryEntries[i].path().filename().string())
					{
						if (mFocusFileRename)
						{
							ImGui::SetKeyboardFocusHere();
							mFocusFileRename = false;
						}
						ImGui::SetNextItemWidth(mCellSize.x - mTextPos.x - mPadding);
						if (ImGui::InputText("##Rename File", mRenameFileBuffer, sizeof(mRenameFileBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							CommitFileRename(directoryEntries[i].path());
						}
						else if (ImGui::IsItemDeactivated())
							mRenamingFilename.clear();
					}
					else
					{
						ImGui::TextUnformatted(entryName.c_str());
						if (ImGui::IsItemHovered())
							ImGui::SetItemTooltip("%s", entryName.c_str());
					}
				}
				ImGui::EndChild();
				if ((i + 1) % cellsPerRow != 0)
					ImGui::SameLine();

				ImGui::PopID();
			}

			if (ImGui::BeginPopupContextWindow("AssetBrowser Context Window", ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Scene"))
				{
					FileSystem::CreateEmpty(mActiveDirectoryPath, "NewScene", Scene::GetExtension());
				}
				ImGui::EndPopup();
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

	bool AssetBrowser::CommitFileRename(const std::filesystem::path& oldPath)
	{
		mRenamingFilename.clear();

		std::string newStemName = mRenameFileBuffer;
		if (!FileSystem::IsValidFileName(newStemName))
			return false;

		std::string newFileName = newStemName + oldPath.extension().string();
		if (newFileName == oldPath.filename().string().c_str())
			return false;

		Scene* currentScene = Application::Get()->GetScene();
		std::error_code ec;
		if (currentScene && std::filesystem::equivalent(oldPath, currentScene->GetPath(), ec))
		{
			LOG_WARN("Can't rename the currently open scene from Asset Browser");
			return false;
		}

		if (FileSystem::RenameFile(oldPath, newFileName))
			return true;

		return false;
	}
}