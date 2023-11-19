#pragma once
#include "ThirdParty/glm/ext/vector_float3.hpp"
#include <vector>
#include <memory>
#include "TGShaderProgram.h"

struct TGPointLight
{
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float mConstant = 1.0f;
	float mLinear = 0.09f;
	float mQuadratic = 0.032f;
};

struct TGSpotLight
{
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 mDirection = glm::vec3(1.0f, 0.0f, 0.0f);

	float mCutoff = 0.5;
	float mConstant = 1.0f;
	float mLinear = 0.09f;
	float mQuadratic = 0.032f;
};

struct TGDirectionLight
{
	glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 mDirection = glm::vec3(1.0f, 0.0f, 0.0f);
};

class TGLightManager
{
public:
	bool AddPointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);

	bool AddDirectionLight(glm::vec3 direction, glm::vec3 color);

	bool AddSpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 direction, float cutoff);

	bool ApplyLightsToShaderPass(std::shared_ptr<TGShaderProgram> program);

private:
	int mMaxPointLightNumber = 4;
	int mMaxDirectionLightNumber = 1;
	int mMaxSpotLightNumber = 2;

	std::vector<TGPointLight> mPointLights;
	std::vector<TGDirectionLight> mDirectionLights;
	std::vector<TGSpotLight> mSpotLights;
};