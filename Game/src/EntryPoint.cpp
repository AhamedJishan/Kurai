#pragma once

#include "Core/Application.h"
#include "Core/Window.h"

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1080;
	config.Height = 720;

	Dawn::Application app(config);
	app.LoadScene("TestScene");
	app.Run();

	return 0;
}