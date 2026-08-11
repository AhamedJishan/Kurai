#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include "Window.h"

namespace Dawn
{
	// Forward declarations
	class Scene;
	class Layer;
	class Window;
	class Renderer;
	class InputSystem;
	class ImGuiSystem;
	class AudioSystem;
	class AssetManager;
	class ComponentFactory;

	struct AppConfig
	{
		glm::vec2 RenderResolution = { 1920, 1080 };
		WindowConfig Window;
	};

	class Application
	{
	public:
		Application(AppConfig appConfig, ComponentFactory* componentFactory);
		virtual ~Application();

		void LoadScene(const std::string& sceneName);
		void SaveScene(const std::string& sceneName);

		void Run();
		void Quit() { mIsRunning = false; }

		static Application* Get() { return sInstance; }

		Scene* GetScene() const { return mScene; }
		Window* GetWindow() const { return mWindow; }
		Renderer* GetRenderer() const { return mRenderer; }
		InputSystem* GetInputSystem() const { return mInputSystem; }
		AudioSystem* GetAudioSystem() const { return mAudioSystem; }
		AssetManager* GetAssetManager() const { return mAssetManager; }
		ComponentFactory* GetComponentFactory() const { return mComponentFactory; }

		void PushLayer(Layer* layer);
		void PopLayer();

	private:
		void Update();
		void GenerateOutput();
		void RegisterBuiltInComponents();

	private:
		static Application* sInstance;

		Scene* mScene = nullptr;
		Scene* mPendingScene = nullptr;

		std::vector<Layer*> mLayerStack;

		Window* mWindow = nullptr;
		Renderer* mRenderer = nullptr;
		InputSystem* mInputSystem = nullptr;
		ImGuiSystem* mImGuiSystem = nullptr;
		AudioSystem* mAudioSystem = nullptr;
		AssetManager* mAssetManager = nullptr;
		ComponentFactory* mComponentFactory = nullptr;

		bool mIsRunning = false;
		double mTime = 0.0;
	};
}