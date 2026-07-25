#pragma once

#include "Core/Application.h"
#include "Core/Window.h"
#include "DawnGame.h"

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1080;
	config.Height = 720;

	Dawn::DawnGame dawn;

	Dawn::Application app(config);
	app.LoadGame(&dawn);
	app.Run();

	return 0;
}