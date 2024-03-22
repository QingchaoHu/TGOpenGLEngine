#pragma once
#include <memory>
#include "ITGSceneItem.h"

class TGScene
{


private:
	std::vector<std::shared_ptr<ITGSceneItem>> mSceneItems;
};