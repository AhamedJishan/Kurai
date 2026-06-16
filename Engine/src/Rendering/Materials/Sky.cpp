#include "Sky.h"

#include "Rendering/Shader.h"

namespace Dawn
{
	Sky::Sky()
		:mTop(glm::vec3(0))
		,mHorizon(glm::vec3(0.7f))
	{
	}

	Sky::~Sky() {}

	const char* Sky::GetName() const
	{
		return "sky";
	}
	
	void Sky::Apply(const Shader* shader)
	{
		shader->SetVec3("u_TopColor", mTop);
		shader->SetVec3("u_HorizonColor", mHorizon);
	}
}