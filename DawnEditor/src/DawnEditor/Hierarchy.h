#pragma once

namespace Dawn
{
	// Forward declarations
	class Actor;

	class Hierarchy
	{
	public:
		void Draw(Actor*& selectedActor);

	private:
		Actor* mActorBeingRenamed = nullptr;
		char mActorRenameBuffer[256] = "";
		bool mFocusActorRename = false;
	};
}