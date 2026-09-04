#include "AssetBrowser.h"

#include <imgui/imgui.h>
#include <filesystem>

namespace Dawn::Editor
{
	void DrawAssetBrwoser()
	{
		ImGui::Begin("Asset Browser");

		std::filesystem::path root = "Assets";

		for (auto& entry : std::filesystem::directory_iterator(root))
		{
			ImGui::PushFont(NULL, 40.0f);
			ImGui::Text(u8"\ue061");
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::Text(entry.path().filename().string().c_str());
		}

		ImGui::End();
	}
}