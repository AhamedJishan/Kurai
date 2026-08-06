#include "Editor.h"

#include <Utils/Log.h>

namespace Dawn
{
	void Editor::OnAttach()
	{
		LOG_INFO("Editor Attached!");
	}

	void Editor::OnDetach()
	{
		LOG_INFO("Editor Detached!");
	}

	void Editor::OnUpdate(float deltaTime)
	{

	}

	void Editor::OnImGuiRender()
	{

	}

}