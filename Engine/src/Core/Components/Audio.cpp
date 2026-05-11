#include "Audio.h"

#include "Audio/SoundEvent.h"
#include "Core/Actor.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"

namespace Dawn
{
	Audio::Audio(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
	}

	Audio::~Audio()
	{
		StopAllEvents();
	}
	
	void Audio::Update(float deltaTime)
	{
		auto iter = mEvents2D.begin();
		while (iter != mEvents2D.end())
		{
			if (!iter->IsValid())
				iter = mEvents2D.erase(iter);
			else
				iter++;
		}

		iter = mEvents3D.begin();
		while (iter != mEvents3D.end())
		{
			if (!iter->IsValid())
				iter = mEvents3D.erase(iter);
			else
			{
				iter->Set3DAttributes(mOwner->GetWorldTransform());
				++iter;
			}
		}
	}
	
	SoundEvent Audio::PlayEvent(const std::string& name)
	{
		SoundEvent event = Application::Get()->GetAudioSystem()->PlayEvent(name);
		if (event.Is3D())
		{
			mEvents3D.emplace_back(event);
			event.Set3DAttributes(mOwner->GetWorldTransform());
		}
		else
			mEvents2D.emplace_back(event);

		return event;
	}
	
	void Audio::StopAllEvents()
	{
		for (SoundEvent event : mEvents2D)
			event.Stop();

		for (SoundEvent event : mEvents3D)
			event.Stop();

		mEvents2D.clear();
		mEvents3D.clear();
	}
}