#pragma once

#include <functional>
#include <memory>

// Forward declaration
struct GLFWwindow;

namespace Dawn
{
	struct WindowConfig
	{
		int Width = 1280;
		int Height = 720;
		const char* Title = "Window-Name";
		const char* IconPath = nullptr;
	};

	class Window
	{
	public:
		using FrameBufferSizeCallbackFn = std::function<void(int width, int height)>;

		Window();
		~Window();

		bool Init(WindowConfig config);

		bool ShouldClose();

		void PollEvents();
		void SwapBuffers();

		void SetFrameBufferSizeCallback(FrameBufferSizeCallbackFn callback) { mFrameBufferSizeCallback = callback; }
		void GetFrameBufferSize(int& width, int& height) const;

		double GetTime();
		void* GetNativeWindow() { return mWindow; }
		int GetWidth() const { return mWindowConfig.Width; }
		int GetHeight() const { return mWindowConfig.Height; }

	private:
		void SetIcon();
		static void OnFrameBufferResize(GLFWwindow* glfwWindow, int width, int height);

	private:
		WindowConfig mWindowConfig;
		GLFWwindow* mWindow = nullptr;
		FrameBufferSizeCallbackFn mFrameBufferSizeCallback;

		std::unique_ptr<class Image> mIcon = nullptr;
	};
}