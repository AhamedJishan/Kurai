#include "Input.h"

#include "Core/Application.h"
#include "InputSystem.h"

namespace Dawn
{
	// ====================================
	//				Keyboard
	// ====================================
	bool Input::GetKey(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKey(key);
	}

	bool Input::GetKeyDown(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKeyDown(key);
	}

	bool Input::GetKeyUp(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKeyUp(key);
	}

	// ====================================
	//				Mouse
	// ====================================
	void Input::SetCursorLocked(bool value)
	{
		Application::Get()->GetInputSystem()->SetCursorLocked(value);
	}

	glm::vec2 Input::GetCursorPos()
	{
		return Application::Get()->GetInputSystem()->GetMouse().GetPosition();
	}

	glm::vec2 Input::GetCursorDeltaPos()
	{
		return Application::Get()->GetInputSystem()->GetMouse().GetDeltaPosition();
	}

	bool Input::GetMouseButton(MouseButton button)
	{
		return Application::Get()->GetInputSystem()->GetMouse().GetButton(button);
	}
	
	bool Input::GetMouseButtonDown(MouseButton button)
	{
		return Application::Get()->GetInputSystem()->GetMouse().GetButtonDown(button);
	}
	
	bool Input::GetMouseButtonUp(MouseButton button)
	{
		return Application::Get()->GetInputSystem()->GetMouse().GetButtonUp(button);
	}
}