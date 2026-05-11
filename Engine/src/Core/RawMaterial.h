#pragma once

#include <string>
#include <glm/vec3.hpp>

namespace Dawn
{
	// CPU-side material specification loaded from a model file (no GPU resources),
	// used to construct runtime Material instances.
	class RawMaterial
	{
	public:
		// Setters
		void SetAlbedoTexturePath(const std::string& albedoTexturePath) { mAlbedoTexturePath = albedoTexturePath; }
		void SetDiffuseTexturePath(const std::string& diffuseTexturePath) { mDiffuseTexturePath = diffuseTexturePath; }
		void SetSpecularTexturePath(const std::string& specularTexturePath) { mSpecularTexturePath = specularTexturePath; }
		void SetNormalTexturePath(const std::string& normalTexturePath) { mNormalTexturePath = normalTexturePath; }
		void SetHeightTexturePath(const std::string& heightTexturePath) { mHeightTexturePath = heightTexturePath; }
		void SetEmissiveTexturePath(const std::string& emissiveTexturePath) { mEmissiveTexturePath = emissiveTexturePath; }

		void SetDiffuseColor(const glm::vec3& diffuseColor) { mDiffuseColor = diffuseColor; }
		void SetSpecularColor(const glm::vec3& specularColor) { mSpecularColor = specularColor; }
		void SetEmissiveColor(const glm::vec3& emissiveColor) { mEmissiveColor = emissiveColor; }

		void SetShininess(float shininess) { mShininess = shininess; }

		// Getters
		const std::string& GetAlbedoTexturePath() const { return mAlbedoTexturePath; }
		const std::string& GetDiffuseTexturePath() const { return mDiffuseTexturePath; }
		const std::string& GetSpecularTexturePath() const { return mSpecularTexturePath; }
		const std::string& GetNormalTexturePath() const { return mNormalTexturePath; }
		const std::string& GetHeightTexturePath() const { return mHeightTexturePath; }
		const std::string& GetEmissiveTexturePath() const { return mEmissiveTexturePath; }

		const glm::vec3& GetDiffuseColor() const { return mDiffuseColor ; }
		const glm::vec3& GetSpecularColor() const { return mSpecularColor ; }
		const glm::vec3& GetEmissiveColor() const { return mEmissiveColor; }

		float GetShininess() const { return mShininess; }

	private:
		std::string mAlbedoTexturePath;
		std::string mDiffuseTexturePath;
		std::string mSpecularTexturePath;
		std::string mNormalTexturePath;
		std::string mHeightTexturePath;
		std::string mEmissiveTexturePath;

		glm::vec3 mDiffuseColor = glm::vec3(1);
		glm::vec3 mSpecularColor = glm::vec3(1);
		glm::vec3 mEmissiveColor = glm::vec3(0);

		float mShininess = 32.0f;
	};
}