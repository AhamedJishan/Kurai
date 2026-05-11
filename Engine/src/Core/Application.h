#pragma once

#include <type_traits>

namespace Dawn
{
	// Forward declarations
	class Game;
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

		void LoadGame(Game* game);

		void Run();
		void Quit() { mIsRunning = false; }

		template<typename T>
		void LoadScene()
		{
			static_assert(std::is_base_of<Scene, T>::value, "T must inherit from Scene!");

			if (mPendingScene)
				delete mPendingScene;

			T* newScene = new T();

			mPendingScene = newScene;
		}


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

		Game* mGame = nullptr;
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