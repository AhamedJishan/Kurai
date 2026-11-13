#include "Viewport.h"

#include <imgui/imgui.h>
#include <Dawn/Rendering/Texture.h>

namespace Dawn
{
	Viewport::Viewport(const char* title)
		:mWindowName(title)
	{}

	void Viewport::DrawText(const char* text)
	{
		if (Begin())
		{
			ImGui::PushFont(NULL, 25.0f);
			ImVec2 availRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(text);

			ImGui::SetCursorPosX((availRegionSize.x - textSize.x) / 2.0f);
			ImGui::SetCursorPosY((availRegionSize.y - textSize.y) / 2.0f);
			ImGui::TextUnformatted(text);
			ImGui::PopFont();
		}
		End();
	}

	void Viewport::DrawTexture(const Texture* texture)
	{
		if (Begin() && texture)
			ImGui::Image((ImTextureID)(intptr_t)(texture->GetId()), {mSize.x, mSize.y}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		End();
	}

	void Viewport::SetFocus()
	{
		ImGui::SetWindowFocus(mWindowName);
	}

	bool Viewport::Begin()
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });
		mIsVisible = ImGui::Begin(mWindowName);

		if (mIsVisible)
		{
			ImVec2 availRegionSize = ImGui::GetContentRegionAvail();
			mIsFocused = ImGui::IsWindowFocused();
			mSize = glm::vec2(availRegionSize.x, availRegionSize.y);
		}
		else
		{
			mIsFocused = false;
			mSize = { 0, 0 };
		}

		return mIsVisible;
	}

	void Viewport::End()
	{
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}