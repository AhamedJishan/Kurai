#include "DawnGame.h"

#include "Core/Application.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

namespace Dawn
{
	DawnGame::DawnGame()
	{
	}

	DawnGame::~DawnGame()
	{
	}
	
	void DawnGame::Start()
	{
		Application::Get()->LoadScene<MainMenuScene>();
	}
}