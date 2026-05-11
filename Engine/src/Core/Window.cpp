#include "Window.h"
#include "Utils/Log.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Rendering/Image.h"

namespace Dawn
{
	const char* TITLE = "Dawn";

	void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ERROR("[GLFW] %s", description);
	}


	Window::Window()
	{
	}

	Window::~Window()
	{
		if (mWindow)
			glfwDestroyWindow(mWindow);

		glfwTerminate();
	}
	
	bool Window::Init(WindowConfig config)
	{
		mWindowConfig = config;

		// GLFW Init
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
		{
			LOG_ERROR("Failed to Initialise glfw");
			return false;
		}
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Window Create
		mWindow = glfwCreateWindow(mWindowConfig.Width, mWindowConfig.Height, mWindowConfig.Title, NULL, NULL);
		if (!mWindow)
		{
			LOG_ERROR("Failed to create window");
			return false;
		}
		glfwMakeContextCurrent(mWindow);
		// Load Opengl
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to Initialize Opengl");
			return false;
		}

		glfwSetWindowUserPointer(mWindow, this);
		glfwSetFramebufferSizeCallback(mWindow, OnFrameBufferResize);

		SetIcon();

		return true;
	}

	void Window::SetIcon()
	{
		if (!mWindowConfig.IconPath)
			return;

		mIcon = std::make_unique<Image>(mWindowConfig.IconPath, 4);

		GLFWimage icons[1];
		icons[0].height = mIcon->GetHeight();
		icons[0].width = mIcon->GetWidth();
		icons[0].pixels = mIcon->GetData();

		glfwSetWindowIcon(mWindow, 1, icons);
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(mWindow);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(mWindow);
	}

	void Window::GetFrameBufferSize(int& width, int& height) const
	{
		glfwGetFramebufferSize(mWindow, &width, &height);
	}

	double Window::GetTime()
	{
		return glfwGetTime();
	}

	// =====================================================
	//					CallBacks
	// =====================================================
	void Window::OnFrameBufferResize(GLFWwindow* glfwWindow, int width, int height)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

		if (window->mFrameBufferSizeCallback)
			window->mFrameBufferSizeCallback(width, height);
	}
}