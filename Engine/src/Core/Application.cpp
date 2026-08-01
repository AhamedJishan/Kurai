#include "Application.h"
#include "Utils/Log.h"

#include "Window.h"
#include "Rendering/Renderer.h"
#include "ImGui/ImGuiSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Asset/AssetManager.h"
#include "SceneSerializer.h"
#include "Scene.h"
// --- COMPONENTS ---
#include "ComponentFactory.h"
#include "Components/Animator.h"
#include "Components/Audio.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
// ------------------

namespace Dawn
{
	Application* Application::sInstance = nullptr;

	Application::Application(WindowConfig windowConfig, ComponentFactory* componentFactory)
		:mIsRunning(true)
		,mTime(0.0)
		,mComponentFactory(componentFactory)
	{
		if (sInstance)
		{
			LOG_ERROR("Application already exists!");
			mIsRunning = false;
			return;
		}
		sInstance = this;

		mWindow = new Window();
		if (!mWindow->Init(windowConfig))
		{
			mIsRunning = false;
			return;
		}

		mAudioSystem = new AudioSystem();
		mInputSystem = new InputSystem();
		mImGuiSystem = new ImGuiSystem();
		mAssetManager = new AssetManager();

		// needs to happen after AssetManager init
		mRenderer = new Renderer();
		if (!mRenderer->Init())
		{
			mIsRunning = false;
			return;
		}

		RegisterBuiltInComponents();
	}

	Application::~Application()
	{
		if (mComponentFactory)	delete mComponentFactory;
		if (mAssetManager)		delete mAssetManager;
		if (mInputSystem)		delete mInputSystem;
		if (mImGuiSystem)		delete mImGuiSystem;
		if (mAudioSystem)		delete mAudioSystem;
		if (mRenderer)			delete mRenderer;
		if (mWindow)			delete mWindow;

		sInstance = nullptr;
	}

	
	void Application::LoadScene(const std::string& sceneName)
	{
		std::string scenePath = "Assets/Scenes/" + sceneName + ".scene";

		Scene* loadedScene = SceneSerializer::Load(scenePath);
		if (loadedScene)
			mPendingScene = loadedScene;
	}

	void Application::Run()
	{
		while (mIsRunning)
		{
			Update();
			GenerateOutput();
		}
	}

	void Application::Update()
	{
		mWindow->PollEvents();

		if (mWindow->ShouldClose())
			mIsRunning = false;

		double currentTime = mWindow->GetTime();
		double deltaTime = currentTime - mTime;
		mTime = currentTime;
		// Prevent large deltaTime jumps
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;

		if (mPendingScene)
		{
			delete mScene;
			mScene = mPendingScene;
			mPendingScene = nullptr;
		}

		mInputSystem->Update();
		mAudioSystem->Update();

		if (mScene)
			mScene->Update(deltaTime);

		mAudioSystem->Update();
	}

	void Application::GenerateOutput()
	{
		if (mScene)
			mRenderer->Draw();
		mWindow->SwapBuffers();
	}

	void Application::RegisterBuiltInComponents()
	{
		mComponentFactory->Register<Animator>("Animator");
		mComponentFactory->Register<Audio>("Audio");
		mComponentFactory->Register<Camera>("Camera");
		mComponentFactory->Register<MeshRenderer>("MeshRenderer");
	}
}