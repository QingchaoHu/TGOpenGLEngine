#include "TGShaderProgram.h"
#include "GL/glew.h"
#include "ThirdParty/glm/gtc/type_ptr.hpp"

TGShaderProgram::TGShaderProgram()
{
	programID = glCreateProgram();
}

bool TGShaderProgram::AddVertexShader(TGVertexShader* vertexShader)
{
	if (vertexShader->bCompileSuccess && !bVertexShaderAdded)
	{
		glAttachShader(programID, vertexShader->mShaderID);
		bVertexShaderAdded = true;
		return true;
	}

	return false;
}

bool TGShaderProgram::AddFragmentShader(TGFragmentShader* fragmentShader)
{
	if (fragmentShader->bCompileSuccess && !bFragmentShaderAdded)
	{
		glAttachShader(programID, fragmentShader->mShaderID);
		bFragmentShaderAdded = true;
		return true;
	}

	return false;
}

bool TGShaderProgram::BindShaderProgram()
{
	if (!bProgramLinked)
	{
		glLinkProgram(programID);

		int success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			return false;
		}
		else
		{
			bProgramLinked = true;
		}

		return true;
	}

	return true;
}

bool TGShaderProgram::UseProgram()
{
	if (!bProgramLinked)
	{
		BindShaderProgram();
	}

	if (bProgramLinked)
	{
		glUseProgram(programID);
		return true;
	}

	return false;
}

void TGShaderProgram::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void TGShaderProgram::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void TGShaderProgram::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void TGShaderProgram::SetVector4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void TGShaderProgram::SetVector3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void TGShaderProgram::SetVector3(const std::string& name, glm::vec3 xyz)
{
	TGShaderProgram::SetVector3(name, xyz.x, xyz.y, xyz.z);
}

void TGShaderProgram::SetMatrix4x4(const std::string& name, float* mat)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, mat);
}

