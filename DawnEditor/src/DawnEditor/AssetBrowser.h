#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <imgui/imgui.h>
#include <Dawn/ImGui/OpenFontIcons.h>

namespace Dawn
{
	class AssetBrowser
	{
	public:
		void Draw();

	private:
		void DrawDirectoryNavigator();
		void DrawDirBreadCrumbs();
		void DrawDirectoryContents(const std::filesystem::path& currentPath);
		const char* GetDirectoryEntryIcon(const std::filesystem::directory_entry& directoryEntry);

	private:
		std::string mSelectedEntryName = "";
		std::filesystem::path mActiveDirectoryPath = "Assets";

		const ImVec2 mCellSize = { 100.0f, 110.0f };
		const float mPadding = 10.0f;
		const float mIconSize = mCellSize.x - 2.0f * mPadding;
		const ImVec2 mTextPos = { mPadding, mIconSize };

		const std::unordered_map<std::string, const char*> mFileExtToIconMap =
		{
			{".frag", ICON_FILE_TEXT}, {".vert", ICON_FILE_TEXT}, {".scene", ICON_FILE_TEXT}, {".mtl", ICON_FILE_TEXT},
			{".png", ICON_IMAGE}, {".jpg", ICON_IMAGE}, {".jpeg", ICON_IMAGE}, {".tif", ICON_IMAGE},
			{".bank", ICON_MUSIC},
			{".ttf", ICON_ITALIC},
			{".dae", ICON_BOX}, {".obj", ICON_BOX}, {".fbx", ICON_BOX}, {".blend", ICON_BOX}
		};
	};
}