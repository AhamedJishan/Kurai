#pragma once

#include "Core/Application.h"
#include "Core/Window.h"
#include <Core/ComponentFactory.h>

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1080;
	config.Height = 720;

	Dawn::ComponentFactory* componentFactory = new Dawn::ComponentFactory();

	Dawn::Application app(config, componentFactory);
	app.LoadScene("TestScene");
	app.Run();

	return 0;
}