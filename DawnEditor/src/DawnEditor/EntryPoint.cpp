#pragma once

#include <Dawn/Core/Application.h>
#include <Dawn/Core/Window.h>
#include <Dawn/Core/ComponentFactory.h>

#include "Editor.h"

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1080;
	config.Height = 720;

	Dawn::ComponentFactory* componentFactory = new Dawn::ComponentFactory();

	Dawn::Application app(config, componentFactory);
	app.PushLayer(new Dawn::Editor());
	app.LoadScene("TestScene");
	app.Run();

	return 0;
}