#pragma once

#include "Core/Game.h"
#include <imgui/imgui.h>

namespace Dawn
{
	class DawnGame : public Game
	{
	public:
		DawnGame();
		~DawnGame();

		void Start() override;
	};
}