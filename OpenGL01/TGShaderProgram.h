#pragma once
#include "TGShader.h"
#include "ThirdParty/glm/glm.hpp"

class TGShaderProgram
{
public:
	TGShaderProgram();

	bool AddVertexShader(TGVertexShader* vertexShader);

	bool AddFragmentShader(TGFragmentShader* fragmentShader);

	bool BindShaderProgram();

	bool UseProgram();

	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetVector4(const std::string& name, float x, float y, float z, float w);

	void SetVector3(const std::string& name, float x, float y, float z);

	void SetVector3(const std::string& name, glm::vec3 xyz);

	void SetMatrix4x4(const std::string& name, float* mat);

public:
	unsigned int programID;

	bool bVertexShaderAdded = false;

	bool bFragmentShaderAdded = false;

	bool bProgramLinked = false;
};