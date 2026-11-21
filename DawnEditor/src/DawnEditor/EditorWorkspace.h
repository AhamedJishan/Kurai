#pragma once

#include <string>

namespace Dawn
{
	class EditorWorkspace
	{
	public:
		void Begin();
		void End();

	private:
		void DrawMainMenuBar();
		void DrawToolbar();
		void DrawMainWorkspaceWindow();

	private:
		const char* mWindowName = "Editor Workspace";
		const char* mDockspaceName = "Main Dockspace";
		const float mToolbarHeight = 32.0f;
		const float mPlayStopButtonWidth = 30.0f;
		const float mPlayStopButtonHeight = 24.0f;

		std::string mPrePlaySceneSnapshot;
		bool mPrePlaySceneWasDirty;
	};
}