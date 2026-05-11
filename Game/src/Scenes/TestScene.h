#pragma once

#include <imgui/imgui.h>
#include "Core/Scene.h"
#include "Audio/SoundEvent.h"

namespace Dawn
{
	// Forward declarations
	class WaveManager;
	class UpgradeManager;
	class Player;

	class TestScene : public Scene
	{
	public:
		TestScene();
		~TestScene();

		void Init() override;
		void Update(float deltaTime) override;
		void ImGuiRender() override;

	private:
		void DrawCrossHair();

	private:
		Player* mPlayer = nullptr;
	};
}