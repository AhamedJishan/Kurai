#pragma once

#include <Dawn/Core/Component.h>

namespace Kurai
{
	class TestComponent : public Dawn::Component
	{
		void Update(float deltaTime) override;
	};
}