#pragma once

#include <imgui/imgui.h>

namespace Dawn
{
	struct Fonts
	{
		static inline ImFont* Regular = nullptr;
		static inline ImFont* Bold = nullptr;
	};


	struct ScopedFont
	{
		ScopedFont(ImFont* font = Fonts::Regular, float size = 0.0f) { ImGui::PushFont(Fonts::Bold, size); }
		~ScopedFont() { ImGui::PopFont(); }
	};
}