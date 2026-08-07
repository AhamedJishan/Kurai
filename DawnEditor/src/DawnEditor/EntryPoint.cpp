#pragma once

#include <Dawn/Core/Application.h>
#include <Dawn/Core/Window.h>
#include <Dawn/Core/ComponentFactory.h>

#include "EditorLayer.h"

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1440;
	config.Height = 810;

	Dawn::ComponentFactory* componentFactory = new Dawn::ComponentFactory();

	Dawn::Application app(config, componentFactory);
	app.PushLayer(new Dawn::EditorLayer());
	app.LoadScene("TestScene");
	app.Run();

	return 0;
}