#pragma once

namespace Dawn
{
	// forward declaration
	class Shader;

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void Apply(const Shader* shader) = 0;
		virtual const char* GetName() const = 0;
	};
}