#include "TGShader.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <sstream>

TGShader::TGShader(std::string shaderFilePath, std::string shaderName, TGShaderType shaderType)
{
	mShaderType = shaderType;

	std::string shaderResourceCode;
	std::ifstream shaderFileStream;

	shaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFileStream.open(shaderFilePath.c_str());
		std::stringstream shaderStringStream;
		shaderStringStream << shaderFileStream.rdbuf();
		shaderFileStream.close();
		shaderResourceCode = shaderStringStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* shaderCode = shaderResourceCode.c_str();

	int success;
	char infoLog[512];

	if (mShaderType == TGShaderType::TGSHADER_VERTEX)
	{
		mShaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (mShaderType == TGShaderType::TGSHADER_FRAGMENT)
	{
		mShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
	{
		throw "TGShader::Construct::Unknown Shader Type!";
	}

	glShaderSource(mShaderID, 1, &shaderCode, NULL);
	glCompileShader(mShaderID);

	glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		bCompileSuccess = false;
		glGetShaderInfoLog(mShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		bCompileSuccess = true;
	}

	mShaderName = shaderName;
}

TGShader::~TGShader()
{
	glDeleteShader(mShaderID);
}

TGShaderType TGShader::GetShaderType()
{
	return mShaderType;
}
