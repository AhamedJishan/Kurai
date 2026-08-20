#pragma once

namespace Dawn
{
	// Forward declarations
	class Actor;

	namespace Editor
	{
		void BeginInspector(Actor* selectedActor);
		void EndInspector();
	}
}