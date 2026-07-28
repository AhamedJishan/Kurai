#pragma once

#include <string>

namespace Dawn
{
	// Forward declarations
	class Scene;
	class Window;
	class Renderer;
	class InputSystem;
	class ImGuiSystem;
	class AudioSystem;
	class AssetManager;

	class Application
	{
	public:
		Application(struct WindowConfig windowConfig);
		virtual ~Application();

		void LoadScene(const std::string& sceneName);

		void Run();
		void Quit() { mIsRunning = false; }

		static Application* Get() { return sInstance; }

		Scene* GetScene() const { return mScene; }
		Window* GetWindow() const { return mWindow; }
		Renderer* GetRenderer() const { return mRenderer; }
		InputSystem* GetInputSystem() const { return mInputSystem; }
		AudioSystem* GetAudioSystem() const { return mAudioSystem; }
		AssetManager* GetAssetManager() const { return mAssetManager; }

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		Scene* mScene = nullptr;
		Scene* mPendingScene = nullptr;

		Window* mWindow = nullptr;
		Renderer* mRenderer = nullptr;
		InputSystem* mInputSystem = nullptr;
		ImGuiSystem* mImGuiSystem = nullptr;
		AudioSystem* mAudioSystem = nullptr;
		AssetManager* mAssetManager = nullptr;

		bool mIsRunning = false;
		double mTime = 0.0;
	};
}