#include "Application.h"
#include "Utils/Log.h"

#include "Game.h"
#include "Window.h"
#include "Rendering/Renderer.h"
#include "ImGui/ImGuiSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "AssetManager.h"
#include "Scene.h"

namespace Dawn
{
	Application* Application::sInstance = nullptr;

	// TODO: Application takes in init parameters like screen size
	Application::Application(WindowConfig windowConfig)
		:mIsRunning(true)
		,mTime(0.0)
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
	}

	Application::~Application()
	{
		if (mAssetManager)	delete mAssetManager;
		if (mInputSystem)	delete mInputSystem;
		if (mImGuiSystem)	delete mImGuiSystem;
		if (mAudioSystem)	delete mAudioSystem;
		if (mRenderer)		delete mRenderer;
		if (mWindow)		delete mWindow;

		sInstance = nullptr;
	}

	void Application::LoadGame(Game* game)
	{
		mGame = game;
	}

	void Application::Run()
	{
		if (mGame) mGame->Start();

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

		mInputSystem->Update();
		mAudioSystem->Update();

		if (mPendingScene)
		{
			if (mScene) delete mScene;

			mScene = mPendingScene;
			mPendingScene = nullptr;
			mScene->Init();
		}

		if (mScene) 
		{
			mScene->Update(deltaTime);
			mScene->UpdateActors(deltaTime);
			mScene->ResolveCollisions();
		}

		mAudioSystem->Update();
	}

	void Application::GenerateOutput()
	{
		mRenderer->Draw();

		mImGuiSystem->BeginFrame();
		if (mScene) mScene->ImGuiRender();
		mImGuiSystem->EndFrame();

		mWindow->SwapBuffers();
	}
}