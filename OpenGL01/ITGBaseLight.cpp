#include "ITGBaseLight.h"
#include "GL/glew.h"

TGLightManager::TGLightManager()
{

}

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

	bIsDirty = true;

	return true;
}

bool TGLightManager::AddDirectionLight(glm::vec3 direction, glm::vec3 color)
{
	if (mDirectionLights.size() >= mMaxDirectionLightNumber) return false;

	TGDirectionLight directionLight;
	directionLight.mColor = color;
	directionLight.mDirection = direction;

	mDirectionLights.push_back(directionLight);

	bIsDirty = true;

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

	bIsDirty = true;

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

bool TGLightManager::ApplyLightsToShaderUsingTexBuffer(std::shared_ptr<TGShaderProgram> program)
{
	if (mLightParamBuf == nullptr && bIsDirty)
	{
		// 总参数数量
		int sumParamCount = mPointLights.size() * 3 + mSpotLights.size() * 5 + mDirectionLights.size() * 2 + 1;

		// 计算贴图所需的行列数
		int size = 1;
		while (size * size < sumParamCount)
		{
			size *= 2;
		}

		// 若贴图 Buffer 没创建或者 Buffer 无法容纳，则创建新的贴图 Buffer
		if (mLightParamBuf == nullptr || mLightBufferSize < size)
		{
			mLightParamBuf = std::make_shared<TGTexture2D>(size, size, ETGDataFormat::ETGDataFormat_RGB32F);
			mLightBufferSize = size;
		}

		float* bufData = new float[size * size * 3];
		float* bufDataPtr = bufData;
		*(bufDataPtr++) = mPointLights.size();
		*(bufDataPtr++) = mDirectionLights.size();
		*(bufDataPtr++) = mSpotLights.size();

		for (int i = 0; i < mPointLights.size(); i++)
		{
			TGPointLight pointLight = mPointLights[i];
			*(bufDataPtr++) = pointLight.mPosition.x;
			*(bufDataPtr++) = pointLight.mPosition.y;
			*(bufDataPtr++) = pointLight.mPosition.z;
			*(bufDataPtr++) = pointLight.mColor.x;
			*(bufDataPtr++) = pointLight.mColor.y;
			*(bufDataPtr++) = pointLight.mColor.z;
			*(bufDataPtr++) = pointLight.mConstant;
			*(bufDataPtr++) = pointLight.mLinear;
			*(bufDataPtr++) = pointLight.mQuadratic;
		}

		for (int i = 0; i < mDirectionLights.size(); i++)
		{
			TGDirectionLight dirLight = mDirectionLights[i];
			*(bufDataPtr++) = dirLight.mColor.x;
			*(bufDataPtr++) = dirLight.mColor.y;
			*(bufDataPtr++) = dirLight.mColor.z;
			*(bufDataPtr++) = dirLight.mDirection.x;
			*(bufDataPtr++) = dirLight.mDirection.y;
			*(bufDataPtr++) = dirLight.mDirection.z;
		}

		for (int i = 0; i < mSpotLights.size(); i++)
		{
			TGSpotLight spotLight = mSpotLights[i];
			*(bufDataPtr++) = spotLight.mPosition.x;
			*(bufDataPtr++) = spotLight.mPosition.y;
			*(bufDataPtr++) = spotLight.mPosition.z;
			*(bufDataPtr++) = spotLight.mColor.x;
			*(bufDataPtr++) = spotLight.mColor.y;
			*(bufDataPtr++) = spotLight.mColor.z;
			*(bufDataPtr++) = spotLight.mDirection.x;
			*(bufDataPtr++) = spotLight.mDirection.y;
			*(bufDataPtr++) = spotLight.mDirection.z;
			*(bufDataPtr++) = spotLight.mConstant;
			*(bufDataPtr++) = spotLight.mLinear;
			*(bufDataPtr++) = spotLight.mQuadratic;
			*(bufDataPtr++) = spotLight.mCutoff;
			*(bufDataPtr++) = 0.0f;
			*(bufDataPtr++) = 0.0f;
		}

		while (bufDataPtr != (bufData + (size * size * 3)))
		{
			*(bufDataPtr++) = 0.0f;
		}

		mLightParamBuf->SetTextureDataF(bufData);
		
	}

	glActiveTexture(GL_TEXTURE3);
	program->SetInt("lightBuf", 3);
	program->SetInt("lightBufSize", mLightBufferSize);
	glBindTexture(GL_TEXTURE_2D, mLightParamBuf->GetID());

	return true;
}
