#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Dawn
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind();
		bool IsValid() const { return mId != 0; }

		// Uniforms
		void SetInt(const std::string& name, int value) const;
		void SetBool(const std::string& name, bool value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloats(const std::string& name, const float* values, int count) const;
		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec3s(const std::string& name, const glm::vec3* values, int count) const;
		void SetVec4s(const std::string& name, const glm::vec4* values, int count) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		bool LoadProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
		unsigned int CreateShader(const std::string& shaderSPath, int glShaderType);
		bool IsCompiled(unsigned int shaderId);
		int GetUniformLocation(const std::string& name) const;

	private:
		unsigned int mId = 0;
	};
}