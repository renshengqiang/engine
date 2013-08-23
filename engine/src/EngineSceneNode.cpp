#include "EngineSceneNode.h"
#include "EngineMoveableObject.h"
#include "EngineCamera.h"

namespace Engine
{
SceneNode:: SceneNode(const std::string &name) :
	Node(name),
	mp_attachedMoveableObject(NULL)
{
}

SceneNode::~SceneNode()
{
	detachObject();
}
SceneNode *SceneNode::createChildSceneNode(const std::string &name)
{
	return  static_cast<SceneNode*>(this->createChild(name));
}

void SceneNode::attachObject(MoveableObject *pObject)
{
	if(mp_attachedMoveableObject && pObject != mp_attachedMoveableObject)
	{
		mp_attachedMoveableObject->detachFromParent();
	}
	mp_attachedMoveableObject = pObject;
	pObject->setParent(this);
}

void SceneNode::detachObject(void)
{
	if(mp_attachedMoveableObject)
	{
		mp_attachedMoveableObject->detachFromParent();
		mp_attachedMoveableObject = NULL;
	}
}

MoveableObject* SceneNode::getAttachedMoveableObject(void)
{
	return mp_attachedMoveableObject;
}

SceneNode* SceneNode::_createChildImpl(const std::string &name)
{
	return new SceneNode(name);
}

void SceneNode::_needUpdate(bool forceParentUpdate)
{
	Node::_needUpdate(forceParentUpdate);
	if(mp_attachedMoveableObject)
	{
		mp_attachedMoveableObject->updateMoved();
	}
}

void SceneNode::_updateBounds(void)
{
	// Reset bounds first
	m_worldAABB.setNull();

	// Update bounds from own attached objects
	if(mp_attachedMoveableObject)
	{
		m_worldAABB = mp_attachedMoveableObject->getBoundingBox();
		m_worldAABB.transformAffine(_getFullTransform());
	}
}

void SceneNode::_findVisibleObjects(Camera *pCamera, RenderQueue &renderQueue)
{
	if(mp_attachedMoveableObject && mp_attachedMoveableObject->visible())
	{
		if(pCamera->isVisible(getWorldBoundingBox(), NULL))
		{
			mp_attachedMoveableObject->addToRenderQueue(pCamera->getProjViewMatrix() * _getFullTransform(), renderQueue);
		}	
	}
	for(ChildNodeIterator it = m_childVec.begin(); it != m_childVec.end(); ++it)
	{
		SceneNode *pChild = static_cast<SceneNode*>(*it);
		pChild->_findVisibleObjects(pCamera, renderQueue);
	}
}

}
