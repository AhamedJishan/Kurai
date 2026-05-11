#include "Sky.h"

#include "Core/Assets.h"
#include "Rendering/Shader.h"

namespace Dawn
{
	Sky::Sky()
		:mTop(glm::vec3(0))
		,mHorizon(glm::vec3(0.7f))
	{
		mShader = Assets::GetShader("sky");
	}

	Sky::~Sky()
	{
	}
	
	void Sky::Apply()
	{
		mShader->SetVec3("u_TopColor", mTop);
		mShader->SetVec3("u_HorizonColor", mHorizon);
	}
}