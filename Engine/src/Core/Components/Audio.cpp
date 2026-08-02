#include "Audio.h"

#include <yaml-cpp/yaml.h>
#include "Audio/SoundEvent.h"
#include "Core/Actor.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"

namespace Dawn
{

	void Audio::Serialize(YAML::Node& node, SerializationContext& serializationContext) const
	{
		for (const SoundEvent& soundEvent : mEvents2D)
			node["Events2D"].push_back(soundEvent.GetName());
		for (const SoundEvent& soundEvent : mEvents3D)
			node["Events3D"].push_back(soundEvent.GetName());
	}

	void Audio::Deserialize(const YAML::Node& node, SerializationContext& serializationContext)
	{
		for (const YAML::Node& eventNode : node["Events2D"])
			PlayEvent(eventNode.as<std::string>());
		for (const YAML::Node& eventNode : node["Events3D"])
			PlayEvent(eventNode.as<std::string>());
	}

	Audio::Audio(Actor* owner)
		:Component(owner)
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
			mEvents3D.emplace_back(event);
			event.Set3DAttributes(mOwner->GetTransform().ToMatrix());
		}
		else
			mEvents2D.emplace_back(event);

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