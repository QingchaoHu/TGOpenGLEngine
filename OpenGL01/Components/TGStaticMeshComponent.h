#pragma once

#include "../ITGSceneItem.h"
#include "../ITGMeshProxy.h"


class TGStaticMeshComponent : public TGSceneItemImpl, public ITGMeshProxy
{



protected:
	glm::mat4 mTransform;

	glm::vec3 mPosition;

	glm::vec3 mRotation;

	glm::vec3 mScale;

	std::shared_ptr<TGModel> mModel;
};