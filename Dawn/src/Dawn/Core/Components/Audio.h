#pragma once

#include "Dawn/Core/Component.h"

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
		Audio(Actor* owner);
		~Audio();

		void Update(float deltaTime) override;

		// name is of the format "BankName:EventName"
		// For ex: "Assets/Audio/Master:audioEvent"
		SoundEvent PlayEvent(const std::string& name);
		void StopAllEvents();

		std::vector<Property> GetProperties() override;
		void OnPropertiesChanged() override;

	private:
		std::vector<SoundEvent> mEvents2D;
		std::vector<SoundEvent> mEvents3D;

		// Scratch buffer for serialization only. Rebuilt in GetProperties() and OnPropertiesChanged(). Do not read directly.
		std::vector<std::string> mEventNames2D;
		std::vector<std::string> mEventNames3D;
	};
}