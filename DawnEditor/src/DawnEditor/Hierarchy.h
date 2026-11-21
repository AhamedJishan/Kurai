#pragma once

#include "EditorContext.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class Hierarchy
	{
	public:
		void Draw(EditorContext& ctx);

	private:
		Actor* mActorBeingRenamed = nullptr;
		char mActorRenameBuffer[256] = "";
		bool mFocusActorRename = false;
	};
}