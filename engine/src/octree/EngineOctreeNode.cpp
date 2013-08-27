#include "EngineOctreeNode.h"
#include "EngineOctree.h"
#include "EngineOctreeSceneManager.h"
#include "EngineMoveableObject.h"

namespace Engine
{
OctreeNode::OctreeNode( SceneManager *creator ) : 
	SceneNode(""), 
	mp_creator(creator),
	m_octant(NULL)
{
}

OctreeNode::OctreeNode( SceneManager *creator, const std::string &name ) : 
	SceneNode(name), 
	mp_creator(creator),
	m_octant(NULL)
{
}

OctreeNode::~OctreeNode()
{
}

OctreeNode *OctreeNode::_createChildImpl(const  std::string &name)
{
	OctreeNode *newNode = new OctreeNode(mp_creator, name);
	
	return newNode;
}

void OctreeNode::_updateBounds( void )
{
	m_worldAABB.setNull();

	// Update bounds from own attached objects
	if(mp_attachedMoveableObject)
	{
		m_worldAABB = mp_attachedMoveableObject->getBoundingBox();
		m_worldAABB.transformAffine(_getFullTransform());
	}

	//update the OctreeSceneManager that things might have moved.
	// if it hasn't been added to the octree, add it, and if has moved
	// enough to leave it's current node, we'll update it.
	if(!m_worldAABB.isNull())
	{
		static_cast<OctreeSceneManager*>(mp_creator) ->_updateOctreeNode(this);
	}
}

/** Since we are loose, only check the center.
*/
bool OctreeNode::_isIn( AxisAlignedBox &box )
{
	// Always fail if not in the scene graph or box is null
	if (box.isNull()) return false;

	// Always succeed if AABB is infinite
	if (box.isInfinite())
		return true;

	Vector3f center = m_worldAABB.getMaximum().midPoint(m_worldAABB.getMinimum());
	Vector3f bmin = box.getMinimum();
	Vector3f bmax = box.getMaximum();

	bool centre = (bmax > center && bmin < center);
	if(!centre) return false;/*此处返回表示该节点需要放到孩子结点中去*/

	// Even if covering the centre line, need to make sure this BB is not large
	// enough to require being moved up into parent. When added, bboxes would
	// end up in parent due to cascade but when updating need to deal with
	// bbox growing too large for this child
	//此处返回false表示需要放到父亲节点中去，因为此空间节点已经无法保证它的体积
	Vector3f octreeSize = bmax - bmin;
	Vector3f nodeSize = m_worldAABB.getMaximum() - m_worldAABB.getMinimum();
	return nodeSize < octreeSize;
}

}
