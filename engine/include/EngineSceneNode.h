#ifndef _ENGINE_SCENE_NODE_H
#define _ENGINE_SCENE_NODE_H
#include <EngineNode.h>
#include <EngineAxisAlignedBox.h>

namespace Engine
{
class MoveableObject;
class Camera;
class RenderQueue;

class SceneNode : public Node
{
public:
	SceneNode(const std::string &name);
	~SceneNode();
	
	SceneNode *createChildSceneNode(const std::string &name);
	
	void attachObject(MoveableObject *pObject);
	void detachObject(void);
	MoveableObject* getAttachedMoveableObject(void);

	inline virtual void _update(bool updateChildren, bool parentHasChanged)
	{
		Node::_update(updateChildren, parentHasChanged);
		_updateBounds();
	}

	void _findVisibleObjects(Camera *pCamera, RenderQueue &renderQueue);
	
	const AxisAlignedBox& getWorldBoundingBox() const
	{
		return m_worldAABB;
	}

private:
	virtual void _needUpdate(bool forceParentUpdate = false);

	//更新node的包围盒
	virtual void _updateBounds(void);

	//创建一个子SceneNode
	virtual SceneNode* _createChildImpl(const std::string &name);
	
protected:
	AxisAlignedBox m_worldAABB;
	MoveableObject *mp_attachedMoveableObject;
};
}

#endif
