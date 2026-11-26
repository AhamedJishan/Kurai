#include "Rotator.h"

#include <glm/glm.hpp>

namespace Kurai
{
	Rotator::Rotator(Dawn::Actor* owner)
		:Dawn::Component(owner)
		,mAngularSpeed(1.0f)
	{
	}

	std::vector<Dawn::Property> Rotator::GetProperties()
	{
		return { {"AngularSpeed", &mAngularSpeed, Dawn::PropertyType::Float}};
	}

	void Rotator::Update(float deltaTime)
	{
		Dawn::Transform& transform = GetOwner()->GetTransform();

		transform.Rotate(glm::radians(mAngularSpeed), glm::vec3(0, 1, 0));
	}
}