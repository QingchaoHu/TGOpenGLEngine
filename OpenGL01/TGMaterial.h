#pragma once
#include <vector>
#include "TGTexture.h"
#include <memory>
#include "TGShaderProgram.h"
#include "GL/glew.h"

class TGMaterial
{
public:
	TGMaterial();

	int ApplyToShader(std::shared_ptr<TGShaderProgram> shader);

	std::shared_ptr<ITGTexture> GetTexture(int index);

	int AddTexture(std::string path, ETGTextureUseType useType);

	int AddTexture(std::shared_ptr<ITGTexture> texture);

	float mShininess = 1.0f;

	float mMetalness = 0.0f;

	float mRoughness = 0.0f;

	glm::vec3 mAmbient = glm::vec3(1.0, 1.0, 1.0);

	glm::vec3 mSpecular = glm::vec3(1.0, 1.0, 1.0);

	glm::vec3 mDiffuse = glm::vec3(1.0, 1.0, 1.0);

	std::vector<std::shared_ptr<ITGTexture>> mTextures;
};

