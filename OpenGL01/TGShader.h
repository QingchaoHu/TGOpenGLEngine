#pragma once
#include <iostream>
#include <fstream>
#include <string>

enum class TGShaderType
{
	TGSHADER_VERTEX,
	TGSHADER_FRAGMENT
};

class TGShader
{
public:
	TGShader(std::string shaderFilePath, std::string shaderName, TGShaderType shaderType);

	~TGShader();

private:
	TGShaderType GetShaderType();

public:
	unsigned int mShaderID;

	bool bCompileSuccess;

protected:
	TGShaderType mShaderType;

	std::string mShaderName;
};

class TGVertexShader : public TGShader
{
public:
	TGVertexShader(std::string shaderFilePath, std::string shaderName) : TGShader(shaderFilePath, shaderName, TGShaderType::TGSHADER_VERTEX)
	{
		
	}
};

class TGFragmentShader : public TGShader
{
public:
	TGFragmentShader(std::string shaderFilePath, std::string shaderName) : TGShader(shaderFilePath, shaderName, TGShaderType::TGSHADER_FRAGMENT)
	{
		
	}
};