#include "EngineMoveableObject.h"
#include "EngineSceneNode.h"

namespace Engine
{
SceneNode* MoveableObject::getParentSceneNode(void)
{
	return static_cast<SceneNode*>(mp_parentNode);
}
}
