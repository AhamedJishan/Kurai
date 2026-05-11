#include "Shader.h"
#include "Utils/Log.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Dawn
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		unsigned int vertexShader = 0;
		unsigned int fragmentShader = 0;

		vertexShader = CreateShader(vertexPath, GL_VERTEX_SHADER);
		if (vertexShader == 0)
		{
			return;
		}

		fragmentShader = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);
		if (fragmentShader == 0)
		{
			glDeleteShader(vertexShader);
			return;
		}

		if (!LoadProgram(vertexShader, fragmentShader))
		{
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		if (mId != 0)
			glDeleteProgram(mId);
	}

	void Shader::Bind()
	{
		if (mId == 0)
		{
			LOG_WARN("Tried to bind and invalid shader program!");
			return;
		}

		glUseProgram(mId);
	}

	bool Shader::LoadProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId)
	{
		mId = glCreateProgram();
		glAttachShader(mId, vertexShaderId);
		glAttachShader(mId, fragmentShaderId);
		glLinkProgram(mId);

		int program_linked;
		glGetProgramiv(mId, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE)
		{
			int log_length = 0;
			char message[1024];
			glGetProgramInfoLog(mId, 1024, &log_length, message);

			LOG_ERROR("Shader program creation Error: %s", message);

			glDeleteProgram(mId);
			mId = 0;

			return false;
		}

		return true;
	}

	unsigned int Shader::CreateShader(const std::string& shaderPath, int glShaderType)
	{
		std::ifstream file(shaderPath);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to open shader file: %s", shaderPath.c_str());
			return 0;
		}
		std::stringstream sstream;
		sstream << file.rdbuf();
		std::string stringContent = sstream.str();
		const char* contents = stringContent.c_str();

		unsigned int shaderId = glCreateShader(glShaderType);
		glShaderSource(shaderId, 1, &contents, NULL);
		glCompileShader(shaderId);
		if (!IsCompiled(shaderId))
		{
			LOG_ERROR("Failed to Compile Shader: %s", shaderPath.c_str());
			glDeleteShader(shaderId);
			return 0;
		}

		return shaderId;
	}

	bool Shader::IsCompiled(unsigned int shaderId)
	{
		int shaderCompiled;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			int log_length = 0;
			char message[1024];
			glGetShaderInfoLog(shaderId, 1024, &log_length, message);
			LOG_ERROR("Compilation Error: %s", message);
			return false;
		}

		return true;
	}


	// Uniforms ------------------------------
	int Shader::GetUniformLocation(const std::string& name) const
	{
		if (mId == 0)
		{
			LOG_WARN("Tried to set uniform '%s' set on invalid shader program", name.c_str());
			return -1;
		}

		int location = glGetUniformLocation(mId, name.c_str());

		// This is needed but not every frame
		// so disabling for now
		//if (location == -1)
		//	LOG_WARN("Uniform not found: %s", name.c_str());

		return location;
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void Shader::SetBool(const std::string& name, bool value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value ? 1 : 0);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, value);
	}

	void Shader::SetFloats(const std::string& name, const float* values, int count) const
	{
		int location = GetUniformLocation(name);
		if (location != -1 && count != 0)
			glUniform1fv(location, count, values);
	}

	void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, value.x, value.y);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetVec3s(const std::string& name, const const glm::vec3* values, int count) const
	{
		int location = GetUniformLocation(name);
		if (location != -1 && count != 0)
			glUniform3fv(location, count, glm::value_ptr(values[0]));
	}

	void Shader::SetVec4s(const std::string& name, const glm::vec4* values, int count) const
	{
		int location = GetUniformLocation(name);
		if (location != -1 && count != 0)
			glUniform4fv(location, count, glm::value_ptr(values[0]));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}