#include "EditorLayer.h"

#include <glm/vec2.hpp>
#include <Dawn/Utils/Log.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Core/SceneSerializer.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderView.h>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	EditorLayer::EditorLayer(const std::filesystem::path& startScenePath)
		:mStartScenePath(startScenePath)
		,mSceneViewport("Scene Viewport")
		,mGameViewport("Game Viewport")
	{
		mSceneViewportRV = std::make_unique<RenderView>(glm::uvec2(2u), false);
		mSceneViewportRV->SetFitMode(RenderView::FitMode::Fill);

		mGameViewportRV = std::make_unique<RenderView>(glm::uvec2(2u), false);
		mGameViewportRV->SetFitMode(RenderView::FitMode::Letterbox);
	}

	void EditorLayer::OnAttach()
	{
		Application::Get()->simulateScene = false;
		if (!mStartScenePath.empty())
			Application::Get()->LoadScene(mStartScenePath);
	}

	void EditorLayer::OnDetach() 
	{
		Application::Get()->simulateScene = true;
	}

	void EditorLayer::OnUpdate(float deltaTime) 
	{
		if (Application::Get()->GetScene())
			mMainCamera = Application::Get()->GetScene()->GetMainCamera();
		else
			mMainCamera = nullptr;

		mEditorCamera.Update();
		mViewportNavigation.Update(deltaTime, mSceneViewport, mEditorCamera, mContext.selectedActor);

		if ( (mContext.mState == EditorState::Edit) && (Input::GetKey(Key::LeftControl) && Input::GetKeyDown(Key::S)) )
			Application::Get()->SaveScene();

		switch (mContext.requestAction)
		{
		case EditorAction::Play: OnPlay(); break;
		case EditorAction::Stop: OnStop(); break;
		default: break;
		}
	}

	void EditorLayer::OnRender()
	{
		if (mSceneViewport.IsVisible())
		{
			glm::uvec2 res = glm::max(glm::uvec2(mSceneViewport.GetSize()), glm::uvec2(2u));
			float aspectRatio = float(res.x) / float(res.y);
			mSceneViewportRV->SetOutputResolution(res);
			mSceneViewportRV->SetCamera(mEditorCamera.transform.position, mEditorCamera.GetView(), mEditorCamera.GetProjection(aspectRatio), res);
			Application::Get()->GetRenderer()->Render(mSceneViewportRV.get());
		}

		if (mGameViewport.IsVisible() && mMainCamera)
		{
			mGameViewportRV->SetOutputResolution(glm::uvec2(mGameViewport.GetSize()));
			mGameViewportRV->SetCamera(mMainCamera->GetPosition(), mMainCamera->GetView(), mMainCamera->GetProjection(), mMainCamera->GetResolution());
			Application::Get()->GetRenderer()->Render(mGameViewportRV.get());
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		mWorkspace.Begin(mContext);

		mHierarchy.Draw(mContext);
		mSceneViewport.DrawTexture(mSceneViewportRV->GetOutputTexture());

		if (mMainCamera)
			mGameViewport.DrawTexture(mGameViewportRV->GetOutputTexture());
		else
			mGameViewport.DrawText("No Main Camera");

		mInspector.Draw(mContext);
		mAssetBrowser.Draw(mContext);
		mEnvironmentSettingsWindow.Draw();

		mWorkspace.End();
	}

	void EditorLayer::OnPlay()
	{
		mContext.requestAction = EditorAction::None;

		Scene* currentScene = Application::Get()->GetScene();
		if (!currentScene || mContext.mState == EditorState::Play)
			return;

		if (!SceneSerializer::Serialize(currentScene, mPrePlaySceneSnapshot))
		{
			LOG_ERROR("Failed to serialize scene. Can not Play!");
			mPrePlaySceneSnapshot.clear();
			return;
		}

		mContext.mState = EditorState::Play;
		Application::Get()->simulateScene = true;
		mPrePlaySceneWasDirty = currentScene->IsDirty();
	}

	void EditorLayer::OnStop()
	{
		mContext.requestAction = EditorAction::None;

		if (mContext.mState == EditorState::Edit) 
			return;

		Scene* restoredScene = SceneSerializer::Deserialize(mPrePlaySceneSnapshot);
		if (!restoredScene)
		{
			LOG_ERROR("Failed to restore scene from snapshot");
			return;
		}
		mPrePlaySceneSnapshot.clear();

		mContext.selectedActor = nullptr;
		mContext.mState = EditorState::Edit;
		Application::Get()->simulateScene = false;
		restoredScene->MarkDirty(mPrePlaySceneWasDirty);
		Application::Get()->LoadScene(restoredScene, Application::Get()->GetScenePath());
	}
}