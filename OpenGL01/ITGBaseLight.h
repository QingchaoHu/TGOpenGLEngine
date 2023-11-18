#pragma once
#include "ThirdParty/glm/ext/vector_float3.hpp"

struct TGBaseLight
{
public:
	glm::vec3 mLocation = glm::vec3(0, 0, 0);
	glm::vec3 mDirection = glm::vec3(1, 0, 0);
	glm::vec3 mColor = glm::vec3(1.0, 1.0, 1.0);
	float mMinAttenuationDistance = 1.0;
	float mMaxAttenuationDistance = 10.0;
	float mStrength = 100.0;
};