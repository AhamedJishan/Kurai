#pragma once

#include "Layer.h"

#include <filesystem>
#include <memory>

namespace Dawn
{
	// forward delcarations
	class RenderView;

	class GameLayer : public Layer
	{
	public:
		explicit GameLayer(const std::filesystem::path& startScenePath);

		void OnAttach() override;
		void OnRender() override;

	private:
		std::filesystem::path mStartScenePath;
		std::unique_ptr<RenderView> mGameRV;
	};
}