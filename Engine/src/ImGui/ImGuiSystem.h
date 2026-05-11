#pragma once

namespace Dawn
{
	class ImGuiSystem
	{
	public:
		ImGuiSystem();
		~ImGuiSystem();

		void BeginFrame();
		void EndFrame();
	};
}