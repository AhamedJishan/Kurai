#pragma once

#include <glm/vec3.hpp>
#include "Rendering/Material.h"

namespace Dawn
{
	// Forward declaration
	class Texture;
	class RawMaterial;

	class PhongMaterial : public Material 
	{
	public:
		PhongMaterial();
		~PhongMaterial();

		static PhongMaterial* CreateFromRaw(const RawMaterial* rawMaterial);

		void Apply() override;

		Texture* GetDiffuseMap() { return mDiffuseMap; }
		Texture* GetSpecularMap() { return mSpecularMap; }
		Texture* GetNormalMap() { return mNormalMap; }
		Texture* GetEmissiveMap() { return mEmissiveMap; }

		glm::vec3 GetDiffuseColor() { return mDiffuseColor; }
		glm::vec3 GetSpecularColor() { return mSpecularColor; }
		glm::vec3 GetEmissiveColor() { return mEmissiveColor; }

		float GetShininess() { return mShininess; }

		void SetDiffuseMap(Texture* texture) { mDiffuseMap = texture; }
		void SetSpecularMap(Texture* texture) { mSpecularMap = texture; }
		void SetNormalMap(Texture* texture) { mNormalMap = texture; }
		void SetEmissiveMap(Texture* texture) { mEmissiveMap = texture; }

		void SetDiffuseColor(const glm::vec3& color) { mDiffuseColor = color; }
		void SetSpecularColor(const glm::vec3& color) { mSpecularColor = color; }
		void SetEmissiveColor(const glm::vec3& color) { mEmissiveColor = color; }

		void SetShininess(float shininess) { mShininess = shininess; }

	private:
		Texture* mDiffuseMap = nullptr;
		Texture* mSpecularMap = nullptr;
		Texture* mNormalMap = nullptr;
		Texture* mEmissiveMap = nullptr;

		glm::vec3 mDiffuseColor = glm::vec3(1.0f);
		glm::vec3 mSpecularColor = glm::vec3(1.0f);
		glm::vec3 mEmissiveColor = glm::vec3(0.0f);

		float mShininess = 32.0f;
	};
}