#include "PhongMaterial.h"

#include "Core/Assets.h"
#include "Core/RawMaterial.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

namespace Dawn
{
	PhongMaterial::PhongMaterial()
	{
		mShader = Assets::GetShader("phong");
	}

	PhongMaterial::~PhongMaterial()
	{
	}

	PhongMaterial* PhongMaterial::CreateFromRaw(const RawMaterial* rawMaterial)
	{
		if (!rawMaterial)
			return nullptr;

		PhongMaterial* phongMaterial = new PhongMaterial();

		Texture* albedoMap = nullptr;
		Texture* diffuseMap = nullptr;
		Texture* specularMap = nullptr;
		Texture* normalMap = nullptr;
		Texture* heightMap = nullptr;
		Texture* emissiveMap = nullptr;

		if (!rawMaterial->GetAlbedoTexturePath().empty())	albedoMap = Assets::GetTexture(rawMaterial->GetAlbedoTexturePath());
		if (!rawMaterial->GetDiffuseTexturePath().empty())	diffuseMap = Assets::GetTexture(rawMaterial->GetDiffuseTexturePath());
		if (!rawMaterial->GetSpecularTexturePath().empty()) specularMap = Assets::GetTexture(rawMaterial->GetSpecularTexturePath());
		if (!rawMaterial->GetNormalTexturePath().empty())	normalMap = Assets::GetTexture(rawMaterial->GetNormalTexturePath());
		if (!rawMaterial->GetHeightTexturePath().empty())	heightMap = Assets::GetTexture(rawMaterial->GetHeightTexturePath());
		if (!rawMaterial->GetEmissiveTexturePath().empty())	emissiveMap = Assets::GetTexture(rawMaterial->GetEmissiveTexturePath());

		phongMaterial->SetDiffuseColor(rawMaterial->GetDiffuseColor());
		phongMaterial->SetSpecularColor(rawMaterial->GetSpecularColor());
		phongMaterial->SetEmissiveColor(rawMaterial->GetEmissiveColor());
		phongMaterial->SetShininess(rawMaterial->GetShininess());

		if (diffuseMap)
			phongMaterial->SetDiffuseMap(diffuseMap);
		else if (albedoMap)	
			phongMaterial->SetDiffuseMap(albedoMap);

		if (specularMap)
			phongMaterial->SetSpecularMap(specularMap);

		if (normalMap) 
			phongMaterial->SetNormalMap(normalMap);
		else if (heightMap)
			phongMaterial->SetNormalMap(heightMap);

		if (emissiveMap)
			phongMaterial->SetEmissiveMap(emissiveMap);

		return phongMaterial;
	}
	
	void PhongMaterial::Apply()
	{
		// Renderer would bind Shader

		if (mDiffuseMap)
		{
			mDiffuseMap->Bind(0);
			mShader->SetInt("u_DiffuseTexture", 0);
			mShader->SetBool("u_HasDiffuseMap", true);
		}
		else
			mShader->SetBool("u_HasDiffuseMap", false);

		if (mSpecularMap)
		{
			mSpecularMap->Bind(1);
			mShader->SetInt("u_SpecularTexture", 1);
			mShader->SetBool("u_HasSpecularMap", true);
		}
		else
			mShader->SetBool("u_HasSpecularMap", false);

		if (mNormalMap)
		{
			mNormalMap->Bind(2);
			mShader->SetInt("u_NormalTexture", 2);
			mShader->SetBool("u_HasNormalMap", true);
		}
		else
			mShader->SetBool("u_HasNormalMap", false);

		if (mEmissiveMap)
		{
			mEmissiveMap->Bind(3);
			mShader->SetInt("u_EmissiveTexture", 3);
			mShader->SetBool("u_HasEmissiveMap", true);
		}
		else
			mShader->SetBool("u_HasEmissiveMap", false);

		mShader->SetVec3("u_DiffuseColor", mDiffuseColor);
		mShader->SetVec3("u_SpecularColor", mSpecularColor);
		mShader->SetVec3("u_EmissiveColor", mEmissiveColor);
		mShader->SetFloat("u_Shininess", mShininess);
	}
}