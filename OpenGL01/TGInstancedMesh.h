#pragma once
#include "TGModel.h"
#include <memory>
#include <glm/glm.hpp>

class TGInstancedMesh
{
public:
	TGInstancedMesh(std::shared_ptr<TGModel> model);

	void Draw();

	void AddInstance();

	void RemoveInstanceAt();

public:
	std::vector<glm::mat4> mTransforms;

	int mPerInstancedCustomDataNumber = 0;

	float* mCustomDataBuffer = nullptr;

	std::shared_ptr<TGModel> mModel;
};