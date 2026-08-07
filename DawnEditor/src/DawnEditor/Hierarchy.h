#pragma once

namespace Dawn
{
	// Forward declarations
	class Actor;

	namespace Editor
	{
		void BeginHierarchy(Actor*& selectedActor);
		void EndHierarchy();
	}
}