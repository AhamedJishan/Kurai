#include "EditorLayer.h"

#include <glm/vec2.hpp>
#include "DockSpace.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
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
		if (!mStartScenePath.empty())
			Application::Get()->LoadScene(mStartScenePath);
	}

	void EditorLayer::OnDetach() {}

	void EditorLayer::OnUpdate(float deltaTime) 
	{
		if (Application::Get()->GetScene())
			mMainCamera = Application::Get()->GetScene()->GetMainCamera();
		else
			mMainCamera = nullptr;

		mEditorCamera.Update();
		mViewportNavigation.Update(deltaTime, mSceneViewport, mEditorCamera, mSelectedActor);

		if (Input::GetKey(Key::LeftControl) && Input::GetKeyDown(Key::S))
			Application::Get()->SaveScene();
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
		Editor::BeginDockSpace();

		mHierarchy.Draw(mSelectedActor);
		mSceneViewport.DrawTexture(mSceneViewportRV->GetOutputTexture());
		if (mMainCamera)
			mGameViewport.DrawTexture(mGameViewportRV->GetOutputTexture());
		else
			mGameViewport.DrawText("No Main Camera");
		mInspector.Draw(mSelectedActor);
		mAssetBrowser.Draw();
		mEnvironmentSettingsWindow.Draw();

		Editor::EndDockSpace();
	}
}