#pragma once

namespace Dawn
{
	// Forward declaration
	class Shader;

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void Apply() = 0;
		virtual Shader* GetShader() const { return mShader; }

	protected:
		Shader* mShader = nullptr;
	};
}