#include "Audio.h"

#include <algorithm>
#include <Dawn/Audio/SoundEvent.h>
#include <Dawn/Core/Actor.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Audio/AudioSystem.h>

namespace Dawn
{
	std::vector<Property> Audio::GetProperties()
	{
		return 
		{
			{"Events2D", &mEventNames2D, PropertyType::StringList},
			{"Events3D", &mEventNames3D, PropertyType::StringList}
		};
	}

	void Audio::OnPropertiesChanged()
	{
		StopAllEvents();

		for (const std::string& eventName : mEventNames2D) PlayEvent(eventName);
		for (const std::string& eventName : mEventNames3D) PlayEvent(eventName);
	}

	Audio::Audio(Actor* owner)
		:Component(owner)
	{
	}

	Audio::~Audio()
	{
		StopAllEvents();
		mEventNames2D.clear();
		mEventNames3D.clear();
	}
	
	void Audio::Update(float deltaTime)
	{
		auto iter = mEvents2D.begin();
		while (iter != mEvents2D.end())
		{
			if (!iter->IsValid())
			{
				auto eventNameItr = std::find(mEventNames2D.begin(), mEventNames2D.end(), iter->GetName());
				if (eventNameItr != mEventNames2D.end())
					mEventNames2D.erase(eventNameItr);
				iter = mEvents2D.erase(iter);
			}
			else
				iter++;
		}

		iter = mEvents3D.begin();
		while (iter != mEvents3D.end())
		{
			if (!iter->IsValid())
			{
				auto eventNameItr = std::find(mEventNames3D.begin(), mEventNames3D.end(), iter->GetName());
				if (eventNameItr != mEventNames3D.end())
					mEventNames3D.erase(eventNameItr);
				iter = mEvents3D.erase(iter);
			}
			else
			{
				iter->Set3DAttributes(mOwner->GetTransform().ToMatrix());
				++iter;
			}
		}
	}
	
	SoundEvent Audio::PlayEvent(const std::string& name)
	{
		SoundEvent event = Application::Get()->GetAudioSystem()->PlayEvent(name);
		if (event.Is3D())
		{
			mEventNames3D.push_back(event.GetName());
			mEvents3D.emplace_back(event);
			event.Set3DAttributes(mOwner->GetTransform().ToMatrix());
		}
		else
		{
			mEventNames2D.push_back(event.GetName());
			mEvents2D.emplace_back(event);
		}

		return event;
	}
	
	void Audio::StopAllEvents()
	{
		for (SoundEvent& event : mEvents2D)
			event.Stop();

		for (SoundEvent& event : mEvents3D)
			event.Stop();

		mEvents2D.clear();
		mEvents3D.clear();
	}
}