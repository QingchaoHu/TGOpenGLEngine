#include "ITGBaseLight.h"

bool TGLightManager::AddPointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
{
	if (mPointLights.size() >= mMaxPointLightNumber) return false;

	TGPointLight pointLight;
	pointLight.mColor = color;
	pointLight.mPosition = position;
	pointLight.mQuadratic = quadratic;
	pointLight.mLinear = linear;
	pointLight.mConstant = constant;

	mPointLights.push_back(pointLight);

	return true;
}

bool TGLightManager::AddDirectionLight(glm::vec3 direction, glm::vec3 color)
{
	if (mDirectionLights.size() >= mMaxDirectionLightNumber) return false;

	TGDirectionLight directionLight;
	directionLight.mColor = color;
	directionLight.mDirection = direction;

	mDirectionLights.push_back(directionLight);

	return true;
}

bool TGLightManager::AddSpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 direction, float cutoff)
{
	if (mSpotLights.size() >= mMaxSpotLightNumber) return false;

	TGSpotLight spotLight;
	spotLight.mColor = color;
	spotLight.mPosition = position;
	spotLight.mDirection = direction;
	spotLight.mCutoff = cutoff;

	mSpotLights.push_back(spotLight);

	return true;
}

bool TGLightManager::ApplyLightsToShaderPass(std::shared_ptr<TGShaderProgram> program)
{
	program->SetInt("actPointLightNumber", mPointLights.size());
	for (int i = 0; i < mPointLights.size(); ++i)
	{
		std::string constantKey = "pointLights[" + std::to_string(i) + "].constant";
		std::string positionKey = "pointLights[" + std::to_string(i) + "].position";
		std::string colorKey = "pointLights[" + std::to_string(i) + "].color";
		std::string linearKey = "pointLights[" + std::to_string(i) + "].linear";
		std::string quadraticKey = "pointLights[" + std::to_string(i) + "].quadratic";

		program->SetVector3(positionKey, mPointLights[i].mPosition);
		program->SetVector3(colorKey, mPointLights[i].mColor);
		program->SetFloat(constantKey, mPointLights[i].mConstant);
		program->SetFloat(linearKey, mPointLights[i].mLinear);
		program->SetFloat(quadraticKey, mPointLights[i].mQuadratic);
	}

	program->SetInt("actDirLightNumber", mDirectionLights.size());
	for (int i = 0; i < mDirectionLights.size(); ++i)
	{
		std::string colorKey = "dirLights[" + std::to_string(i) + "].color";
		std::string directionKey = "dirLights[" + std::to_string(i) + "].direction";

		program->SetVector3(directionKey, mDirectionLights[i].mDirection);
		program->SetVector3(colorKey, mDirectionLights[i].mColor);
	}

	program->SetInt("actSpotLightNumber", mSpotLights.size());
	for (int i = 0; i < mSpotLights.size(); ++i)
	{
		std::string constantKey = "mSpotLights[" + std::to_string(i) + "].constant";
		std::string positionKey = "mSpotLights[" + std::to_string(i) + "].position";
		std::string directionKey = "mSpotLights[" + std::to_string(i) + "].direction";
		std::string cutoffKey = "mSpotLights[" + std::to_string(i) + "].cutoff";
		std::string colorKey = "mSpotLights[" + std::to_string(i) + "].color";
		std::string linearKey = "mSpotLights[" + std::to_string(i) + "].linear";
		std::string quadraticKey = "mSpotLights[" + std::to_string(i) + "].quadratic";

		program->SetVector3(positionKey, mSpotLights[i].mPosition);
		program->SetVector3(colorKey, mSpotLights[i].mColor);
		program->SetVector3(directionKey, mSpotLights[i].mDirection);
		program->SetFloat(cutoffKey, mSpotLights[i].mCutoff);
		program->SetFloat(constantKey, mSpotLights[i].mConstant);
		program->SetFloat(linearKey, mSpotLights[i].mLinear);
		program->SetFloat(quadraticKey, mSpotLights[i].mQuadratic);
	}

	return true;
}
