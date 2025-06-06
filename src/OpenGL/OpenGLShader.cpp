#include "agipch.hpp"
#include "OpenGLShader.hpp"

#include <fstream>
#include <array>

#include <glm/gtc/type_ptr.hpp>

namespace AGI {

	namespace Utils {

		static GLenum ShaderTypeToGLType(ShaderType type)
		{
			if (type == ShaderType::Vertex)   return GL_VERTEX_SHADER;
			if (type == ShaderType::Fragment) return GL_FRAGMENT_SHADER;

			AGI_VERIFY(false, "Unknown shader type '{}'", (int)type);
			return GL_NONE;
		}

	}

	OpenGLShader::OpenGLShader(const ShaderSources& shaderSources)
	{
		AGI_VERIFY(shaderSources.size() <= 2, "We only support 2 shaders for now");

		m_RendererID = glCreateProgram();

		std::vector<GLuint> shaderIDs(shaderSources.size());
		int shaderIndex = 0;

		for (const auto& [type, source] : shaderSources)
		{
			GLuint shader = Compile(Utils::ShaderTypeToGLType(type), source);
			glAttachShader(m_RendererID, shader);
			shaderIDs[shaderIndex++] = shader;
		}

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);

		if (!isLinked)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

			AGI_ERROR("{0}", infoLog.data());
			AGI_VERIFY(false, "Shader link failure!");
			return;
		}

		for (GLuint id : shaderIDs)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	bool OpenGLShader::AttributeExists(const std::string& name) const
	{
		return GetLocation(name.c_str()) == -1 ? false : true;
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		Bind();
		glUniform1i(GetLocation(name.c_str()), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		Bind();
		glUniform1iv(GetLocation(name.c_str()), count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		Bind();
		glUniform1f(GetLocation(name.c_str()), value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		Bind();
		glUniform2f(GetLocation(name.c_str()), value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		Bind();
		glUniform3f(GetLocation(name.c_str()), value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		Bind();
		glUniform4f(GetLocation(name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		Bind();
		glUniformMatrix3fv(GetLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		Bind();
		glUniformMatrix4fv(GetLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	GLint OpenGLShader::GetLocation(const char* attr) const
	{
		return glGetUniformLocation(m_RendererID, attr);
	}

	GLuint OpenGLShader::Compile(GLenum type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);
		const GLchar* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
			glDeleteShader(shader);

			AGI_ERROR("{0}", infoLog.data());
			AGI_VERIFY(false, "Shader compilation failure!");
		}

		return shader;
	}

}