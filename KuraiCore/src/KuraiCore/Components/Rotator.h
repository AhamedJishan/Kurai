#pragma once

#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Component.h>
#include <Dawn/Core/Property.h>

namespace Kurai
{
	class Rotator : public Dawn::Component
	{
	public:
		Rotator(Dawn::Actor* owner);

		std::vector<Dawn::Property> GetProperties() override;

		void Update(float deltaTime) override;

	private:
		// in degress
		float mAngularSpeed = 1.0f;
	};
}