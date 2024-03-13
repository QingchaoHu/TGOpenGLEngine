#pragma once
#include "TGModel.h"
#include <memory>
#include "ThirdParty/glm/glm.hpp"

class TGDrawInstanceProxy
{
public:
	std::shared_ptr<TGTexture2D> mTransformBufferTex;

	std::shared_ptr<TGTexture2D> mCustomBufferTex;

	int mInstanceNumber = 0;

	int mPerInstanceCustomDataNumber = 0;
};

class TGInstancedModel
{
public:
	TGInstancedModel(std::shared_ptr<TGModel> model);

	void Draw(std::shared_ptr<TGShaderProgram> shader);

	void AddInstance(glm::mat4 transform);

	void RemoveInstanceAt(int index);

	void AddInstanceArray(std::vector<glm::mat4> transformArray);

private:
	void GenerateInstanceRenderTexture();

public:
	std::vector<glm::mat4> mTransforms;

	int mPerInstancedCustomDataNumber = 0;

	float* mCustomDataBuffer = nullptr;

	std::shared_ptr<TGModel> mModel;

	std::shared_ptr<TGTexture2D> mTransformBufferTex;

	std::shared_ptr<TGTexture2D> mCustomBufferTex;

	bool bIsDirty = false;
};