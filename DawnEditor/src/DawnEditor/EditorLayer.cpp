#include "EditorLayer.h"

#include <glm/vec2.hpp>
#include "DockSpace.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderView.h>
#include <Dawn/Rendering/Texture.h>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	EditorLayer::EditorLayer(const std::filesystem::path& startScenePath)
		:mStartScenePath(startScenePath)
	{
		// temp res. Actual value would be updated in OnUpdate()
		mViewportRV = std::make_unique<RenderView>(glm::uvec2(2u), false);
	}

	void EditorLayer::OnAttach()
	{
		if (!mStartScenePath.empty())
			Application::Get()->LoadScene(mStartScenePath);
	}

	void EditorLayer::OnDetach() {}

	void EditorLayer::OnUpdate(float deltaTime) 
	{
		mCamera.Update();
		mViewportNavigation.Update(deltaTime, mViewport, mCamera, mSelectedActor);

		if (Input::GetKey(Key::LeftControl) && Input::GetKeyDown(Key::S))
			Application::Get()->SaveScene();
	}

	void EditorLayer::OnRender()
	{
		float aspectRatio = mViewport.GetSize().x / mViewport.GetSize().y;
		mViewportRV->SetOutputResolution(glm::uvec2(mViewport.GetSize()));
		mViewportRV->SetCamera(mCamera.transform.position, mCamera.GetView(), mCamera.GetProjection(aspectRatio), glm::uvec2(mViewport.GetSize()));
		Application::Get()->GetRenderer()->Render(mViewportRV.get());
	}

	void EditorLayer::OnImGuiRender()
	{
		Editor::BeginDockSpace();

		mHierarchy.Draw(mSelectedActor);
		mViewport.Draw(mViewportRV->GetOutputTexture());
		mInspector.Draw(mSelectedActor);
		mAssetBrowser.Draw();
		mEnvironmentSettingsWindow.Draw();

		Editor::EndDockSpace();
	}

}