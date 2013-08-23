#ifndef _ENGINE_MOVEABLE_OBJECT_H
#define _ENGINE_MOVEABLE_OBJECT_H
#include <EngineAxisAlignedBox.h>

namespace Engine
{
class Node;
class SceneNode;
class RenderQueue;
class Matrix4f;
//MoveableObject���ǿ��Ը��ݳ����ڵ㵽���ƶ��Ľڵ�
class MoveableObject
{
public:
	inline MoveableObject():
		mp_parentNode(NULL),
		m_visible(false),
		m_moved(false)
	{
		m_boundingBox.setExtents(Vector3f(0,0,0), Vector3f(0,0,0));
	}
	inline virtual ~MoveableObject()
	{
		detachFromParent();
	}

	inline Node* getParentNode(void)
	{
		return mp_parentNode;
	}
	
	SceneNode* getParentSceneNode(void);
	
	inline void detachFromParent(void)
	{
		mp_parentNode = NULL;
	}

	inline virtual void setParent(Node *pParentNode)
	{
		mp_parentNode = pParentNode;
	}

	inline void updateMoved(void)
	{
		m_moved = true;
	}
	
	bool isMovded(void)
	{
		return m_moved;
	}
	
	bool visible(void) const
	{
		return m_visible;
	}
	void setVisible(bool visible)
	{	
		m_visible = visible;
	}

	//SceneManagerʹ����һ������Moveable�е�Renderable���뵽��Ⱦ������
	virtual void addToRenderQueue(const Matrix4f &transMatrix, RenderQueue &renderQueue){};

	//���ؾֲ�����ϵ�İ�Χ��
	virtual const AxisAlignedBox& getBoundingBox(void) const { return m_boundingBox;}
	
protected:
	Node	*mp_parentNode;
	bool		m_visible;	
	bool		m_moved;
	AxisAlignedBox m_boundingBox;
};
}
#endif

