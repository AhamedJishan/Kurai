#pragma once

namespace Dawn
{
	// Base class for the client game logic.
	// The Engine's Application needs a Game instance to drives its lifecycle. 
	// Inherit from this to define specific global state, that persists beyond scenes, for entirety of the Game Life.
	class Game
	{
	public:
		virtual ~Game() {}

		virtual void Start() {}
	};
}