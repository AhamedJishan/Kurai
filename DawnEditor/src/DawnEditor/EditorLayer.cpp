#include "EditorLayer.h"

#include <glm/vec2.hpp>
#include "DockSpace.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderTarget.h>
#include <Dawn/Rendering/Texture.h>
#include <Dawn/Input/Input.h>

namespace Dawn
{
	void EditorLayer::OnAttach() 
	{
		RenderTargetSpecification rtSpec;
		rtSpec.resolution = Application::Get()->GetRenderer()->GetResolution();
		rtSpec.colorAttachment = TextureFormat::RGBA8;
		mViewportRT = new RenderTarget(rtSpec);

		Application::Get()->GetRenderer()->SetOutputRenderOutput(mViewportRT);
	}

	void EditorLayer::OnDetach() 
	{
		delete mViewportRT;
		Application::Get()->GetRenderer()->SetOutputRenderOutput(nullptr);
	}

	void EditorLayer::OnUpdate(float deltaTime) 
	{
		mCamera.Update();
		mViewportRT->SetResolution(glm::uvec2(mViewport.GetSize()));
		mViewportNavigation.Update(deltaTime, mViewport, mCamera, mSelectedActor);

		if (Input::GetKey(Key::LeftControl) && Input::GetKeyDown(Key::S))
			Application::Get()->SaveScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		Editor::BeginDockSpace();

		mHierarchy.Draw(mSelectedActor);
		mViewport.Draw(mViewportRT->GetColorTexture());
		mInspector.Draw(mSelectedActor);
		mAssetBrowser.Draw();
		mEnvironmentSettingsWindow.Draw();

		Editor::EndDockSpace();
	}

}