#pragma once

#include <string>
#include "EditorContext.h"

namespace Dawn
{
	class EditorWorkspace
	{
	public:
		void Begin(EditorContext& ctx);
		void End();

	private:
		void DrawMainMenuBar();
		void DrawToolbar(EditorContext& ctx);
		void DrawMainWorkspaceWindow();

	private:
		const char* mWindowName = "Editor Workspace";
		const char* mDockspaceName = "Main Dockspace";
		const float mToolbarHeight = 32.0f;
		const float mPlayStopButtonWidth = 30.0f;
		const float mPlayStopButtonHeight = 24.0f;
	};
}