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

	class GameScene : public Scene
	{
	public:
		GameScene();
		~GameScene();

		void Init() override;
		void Update(float deltaTime) override;
		void ImGuiRender() override;

		void SetPlayerHealth(float health) { mPlayerHealth = health; }
		void SetMaxPlayerHealth(float health) { mMaxPlayerHealth = health; }
		void GameOver();

	private:
		void DrawPauseWindow();
		void DrawHealthBar();
		void DrawCrossHair();
		void DrawGameOver(bool win);
		void DrawWaveUI();

	private:
		WaveManager* mWaveManager = nullptr;
		UpgradeManager* mUpgradeManager = nullptr;
		Player* mPlayer = nullptr;

		ImFont* mFontBold = nullptr;
		ImFont* mFontRegular = nullptr;
		ImFont* mFontLight = nullptr;

		float mMaxPlayerHealth = 0.0f;
		float mPlayerHealth = 0.0f;
		bool mIsGameOver = false;

		float mTimeSinceGameOver = 0.0f;
		const float mGameOverTransitionDuration = 2.0f;

		SoundEvent mBgmSE;

		bool mResumeButtonHovered = false;
		bool mRestartButtonHovered = false;
		bool mMainMenuButtonHovered = false;
	};
}