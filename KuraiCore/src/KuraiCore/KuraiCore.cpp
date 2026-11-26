#include "KuraiCore.h"

#include <Dawn/Utils/Log.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Core/ComponentFactory.h>
#include "Components/Rotator.h"

namespace Kurai
{
	void RegisterComponents()
	{
		Dawn::Application* app = Dawn::Application::Get();
		if (!app)
		{
			LOG_ERROR("RegisterComponent() can not be called before Dawn::Application creation!");
			return;
		}

		app->GetComponentFactory()->Register<Rotator>("Rotator");
	}
}