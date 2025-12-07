#include "TGMaterial.h"

TGMaterial::TGMaterial()
{

}

int TGMaterial::ApplyToShader(std::shared_ptr<TGShaderProgram> shader)
{
	for (int i = 0; i < 32; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);
	}

	int diffuseTextureNumber = 0;
	int specularTextureNumber = 0;
	int ambientTextureNumber = 0;
	int shininessTextureNumber = 0;
	int metalnessTextureNumber = 0;
	int roughnessTextureNumber = 0;
	int normalTextureNumber = 0;
	int bumpTextureNumber = 0;
	int diffuseCubeTextureNumber = 0;

	for (int i = 0; i < mTextures.size(); i++)
	{
		std::shared_ptr<ITGTexture> oneTexture = mTextures[i];

		int activedTexNumber = diffuseTextureNumber + ambientTextureNumber + specularTextureNumber + shininessTextureNumber + metalnessTextureNumber + roughnessTextureNumber + normalTextureNumber + bumpTextureNumber + diffuseCubeTextureNumber;

		glActiveTexture(GL_TEXTURE0 + activedTexNumber);
		glEnable(GL_TEXTURE_2D);

		if (oneTexture->GetTextureType() == ETGTextureType_Texture2D)
		{	
			ETGTextureUseType useType = oneTexture->GetUseType();
			if (useType == ETGTextureUseType_Diffuse && diffuseTextureNumber < 3)
			{
				shader->SetInt("material.texture_diffuse" + std::to_string(diffuseTextureNumber), activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				diffuseTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Specular && specularTextureNumber == 0)
			{
				shader->SetInt("material.texture_specular", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				specularTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Ambient && ambientTextureNumber == 0)
			{
				shader->SetInt("material.texture_ambient", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				ambientTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Normal && normalTextureNumber == 0)
			{
				shader->SetInt("material.texture_normal", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				normalTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Bump && bumpTextureNumber == 0)
			{
				shader->SetInt("material.texture_bump", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				bumpTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Metalness && metalnessTextureNumber == 0)
			{
				shader->SetInt("material.texture_metalness", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				metalnessTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Roughness && roughnessTextureNumber == 0)
			{
				shader->SetInt("material.texture_roughness", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				roughnessTextureNumber++;
			}
			else if (useType == ETGTextureUseType_Shininess && shininessTextureNumber == 0)
			{
				shader->SetInt("material.texture_shininess", activedTexNumber);
				glBindTexture(GL_TEXTURE_2D, oneTexture->GetID());
				shininessTextureNumber++;
			}
		}
		else
		{
			shader->SetInt("material.texture_round", activedTexNumber);
			glBindTexture(GL_TEXTURE_CUBE_MAP, oneTexture->GetID());
			diffuseCubeTextureNumber++;
		}
	}

	shader->SetInt("material.diffuseTexNumber", diffuseTextureNumber);
	shader->SetInt("material.specularTexNumber", specularTextureNumber);
	shader->SetInt("material.ambientTexNumber", ambientTextureNumber);
	shader->SetInt("material.shininessTexNumber", shininessTextureNumber);
	shader->SetInt("material.metalnessTexNumber", metalnessTextureNumber);
	shader->SetInt("material.roughnessTexNumber", roughnessTextureNumber);
	shader->SetInt("material.normalTexNumber", normalTextureNumber);
	shader->SetInt("material.bumpTexNumber", bumpTextureNumber);

	if (shininessTextureNumber == 0)
	{
		shader->SetFloat("material.shininess", mShininess);
	}

	if (metalnessTextureNumber == 0)
	{
		shader->SetFloat("material.metalness", mMetalness);
	}

	if (roughnessTextureNumber == 0)
	{
		shader->SetFloat("material.roughness", mRoughness);
	}

	if (ambientTextureNumber == 0)
	{
		shader->SetVector3("material.ambient", mAmbient);
	}

	if (specularTextureNumber == 0)
	{
		shader->SetVector3("material.specular", mSpecular);
	}

	if (diffuseTextureNumber == 0)
	{
		shader->SetVector3("material.diffuse", mDiffuse);
	}

	return diffuseTextureNumber + ambientTextureNumber + specularTextureNumber + shininessTextureNumber + metalnessTextureNumber + roughnessTextureNumber + normalTextureNumber + bumpTextureNumber + diffuseCubeTextureNumber;
}

std::shared_ptr<ITGTexture> TGMaterial::GetTexture(int index)
{
	index = index % mTextures.size();
	return mTextures[index];
}

int TGMaterial::AddTexture(std::string path, ETGTextureUseType useType)
{
	std::shared_ptr<ITGTexture> texture = std::make_shared<TGTexture2D>(path, useType);
	return AddTexture(texture);
}

int TGMaterial::AddTexture(std::shared_ptr<ITGTexture> texture)
{
	int slotIndex = mTextures.size();
	mTextures.push_back(texture);
	return slotIndex;
}
