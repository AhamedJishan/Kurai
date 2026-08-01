#pragma once

#include "Core/Component.h"

#include <string>
#include <vector>

namespace Dawn
{
	// Forward declarations
	class Actor;
	class SoundEvent;

	class Audio : public Component
	{
	public:
		~Audio();

		void Update(float deltaTime) override;

		SoundEvent PlayEvent(const std::string& name);
		void StopAllEvents();

		void Serialize(YAML::Node& node) const override;
		void Deserialize(const YAML::Node& node) override;

	protected:
		Audio(Actor* owner);

	private:
		std::vector<SoundEvent> mEvents2D;
		std::vector<SoundEvent> mEvents3D;
	};
}