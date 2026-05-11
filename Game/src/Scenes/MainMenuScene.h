#pragma once

#include <imgui/imgui.h>
#include "Core/Scene.h"
#include "Audio/SoundEvent.h"

namespace Dawn
{
	// Forward declarations
	class Texture;

	class MainMenuScene : public Scene
	{
	public:
		MainMenuScene();
		~MainMenuScene();

		void Init() override;
		void Update(float deltaTime) override;
		void ImGuiRender() override;

	private:
		void DrawMainMenu();

	private:
		ImFont* mFontBold = nullptr;
		ImFont* mFontRegular = nullptr;
		ImFont* mFontLight = nullptr;

		SoundEvent mBgmSE;
		
		bool mStartButtonHovered = false;
		bool mQuitButtonHovered = false;
	};
}