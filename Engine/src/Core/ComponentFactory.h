#pragma once

#include <type_traits>
#include <typeindex>
#include <string>
#include <functional>
#include <unordered_map>
#include <Utils/Log.h>
#include "Actor.h"
#include "Component.h"

namespace Dawn
{
	class ComponentFactory
	{
	public:
		template<typename T>
		void Register(const std::string& componentName)
		{
			static_assert(std::is_base_of_v<Component, T>, "Tried to Register a non-component.");
			static_assert(std::is_constructible_v<T, Actor*>, "Components must provide a constructor of type: T(Actor*).");
			
			mFactories[componentName] = [](Actor* owner)
				{
					return owner->CreateComponent<T>();
				};
			mComponentNames[typeid(T)] = componentName;
		}

		Component* Create(const std::string& componentName, Actor* owner) const
		{
			auto it = mFactories.find(componentName);
			if (it == mFactories.end())
			{
				LOG_ERROR("Component '%s', wasn't registered with the Component Factory!", componentName.c_str());
				return nullptr;
			}

			return it->second(owner);
		}

		std::string GetComponentName(Component* component)
		{
			if (!component)
			{
				LOG_ERROR("Tried to get component name of nullptr.");
				return "";
			}

			auto it = mComponentNames.find(typeid(*component));
			if (it == mComponentNames.end())
			{
				LOG_ERROR("Component of type '%s' hasn't been registered!", typeid(*component).name());
				return "";
			}
			return it->second;
		}

	private:
		std::unordered_map<std::string, std::function<Component* (Actor*)>> mFactories;
		std::unordered_map<std::type_index, std::string> mComponentNames;
	};
}