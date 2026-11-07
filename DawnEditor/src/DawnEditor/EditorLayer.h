#pragma once

#include <memory>
#include <filesystem>
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
	class RenderView;

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::filesystem::path& startScenePath = "");

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::filesystem::path mStartScenePath;
		Actor* mSelectedActor = nullptr;
		std::unique_ptr<RenderView> mViewportRV;

		EditorCamera mCamera;
		ViewportNavigation mViewportNavigation;

		Viewport mViewport;
		Hierarchy mHierarchy;
		Inspector mInspector;
		AssetBrowser mAssetBrowser;
		EnvironmentSettingsWindow mEnvironmentSettingsWindow;
	};
}