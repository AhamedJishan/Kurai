#pragma once
#include <Dawn/Core/Layer.h>
#include "EditorCamera.h"
#include "Viewport.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "AssetBrowser.h"
#include "ViewportNavigation.h"
#include "EnvironmentSettingsWindow.h"

namespace Dawn
{
	// Forward declarations
	class Actor;
	class RenderTarget;

	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		Actor* mSelectedActor = nullptr;
		RenderTarget* mViewportRT = nullptr;

		EditorCamera mCamera;
		ViewportNavigation mViewportNavigation;

		Viewport mViewport;
		Hierarchy mHierarchy;
		Inspector mInspector;
		AssetBrowser mAssetBrowser;
		EnvironmentSettingsWindow mEnvironmentSettingsWindow;
	};
}