#pragma once

#include <imgui/imgui.h>

namespace Dawn::GUI
{
	void ApplyTheme()
	{
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // --- Shape ---------------------------------------------------------
        style.WindowRounding = 4.0f;
        style.ChildRounding = 4.0f;
        style.FrameRounding = 3.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 4.0f;

        style.WindowPadding = ImVec2(8, 8);
        style.FramePadding = ImVec2(6, 4);
        style.ItemSpacing = ImVec2(6, 6);
        style.ItemInnerSpacing = ImVec2(6, 4);
        style.IndentSpacing = 20.0f;
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 10.0f;

        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;

        // --- Colors: dark slate, warm orange accent -------------------------
        colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.52f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.29f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.11f, 0.75f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.39f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.42f, 0.46f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.65f, 0.30f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.31f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.27f, 0.31f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.90f, 0.55f, 0.24f, 0.80f);
        colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.90f, 0.55f, 0.24f, 0.60f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.31f, 0.50f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.55f, 0.24f, 0.60f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.31f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.19f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.90f, 0.55f, 0.24f, 0.60f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.65f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.65f, 0.30f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.55f, 0.24f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.90f, 0.55f, 0.24f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.90f, 0.55f, 0.24f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void ResetTheme()
	{
		ImGui::StyleColorsDark();
	}
}