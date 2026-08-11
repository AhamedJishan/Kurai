#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/vec2.hpp>
#include "Theme.h"
#include "DockSpace.h"
#include "Hierarchy.h"
#include "Viewport.h"
#include "Inspector.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderTarget.h>
#include <Dawn/Rendering/Texture.h>

namespace Dawn
{
	void EditorLayer::OnAttach() 
	{
		mViewportRT = new RenderTarget();
		mViewportTexture = new Texture(1920, 1080, TextureFormat::RGBA8);
		Application::Get()->GetRenderer()->SetOutputRenderOutput(mViewportRT, mViewportTexture);
	}

	void EditorLayer::OnDetach() 
	{
		delete mViewportRT;
		delete mViewportTexture;
		Application::Get()->GetRenderer()->SetOutputRenderOutput(nullptr, nullptr);
	}
	void EditorLayer::OnUpdate(float deltaTime) {}

	void EditorLayer::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		Editor::ApplyTheme();

		Editor::BeginDockSpace();

		Editor::BeginHierarchy(mSelectedActor);
		Editor::EndHierarchy();

		Editor::BeginViewport(mViewportTexture);
		Editor::EndViewport();

		Editor::BeginInspector();
		Editor::EndInspector();
		
		Editor::EndDockSpace();

		Editor::ResetTheme();
	}

}