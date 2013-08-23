#include "EngineNode.h"
#include <stdio.h>

namespace Engine
{
Node::Node():
	mp_parent(0),
	m_orientation(0,0,0,1),
	m_position(0,0,0),
	m_scale(1,1,1),
	m_initialOrientation(0, 0, 0, 1),
	m_initialPosition(0,0,0),
	m_initialScale(1,1,1),
	m_derivedOrientation(0,0,0,1),
	m_derivedPosition(0,0,0),
	m_derivedScale(1,1,1),
	m_needSelfUpdate(false),
	m_needChildUpdate(false),
	m_parentNotified(false),
	m_cachedTransformOutOfDate(false)
{
	_needUpdate();
}

Node::Node(const std::string& name):
		m_name(name),
		mp_parent(0),
		m_orientation(0,0,0,1),
		m_position(0,0,0),
		m_scale(1,1,1),
		m_initialOrientation(0,0,0,1),
		m_initialPosition(0,0,0),
		m_initialScale(1,1,1),
		m_derivedOrientation(0,0,0,1),
		m_derivedPosition(0,0,0),
		m_derivedScale(1,1,1),
		m_needSelfUpdate(false),
		m_needChildUpdate(false),
		m_cachedTransformOutOfDate(false)
{
	_needUpdate();
}

Node::~Node()
{
	for(ChildNodeIterator iter=m_childVec.begin(); iter!=m_childVec.end(); ++iter)
	{
		delete (*iter);
	}
	m_childVec.clear();
}

void Node::reset(void)
{
	m_position = Vector3f(0.0f, 0.0f, 0.0f);
	m_scale = Vector3f(1.0f, 1.0f, 1.0f);
	m_orientation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	for(ChildNodeIterator iter = m_childVec.begin(); iter != m_childVec.end(); ++iter)
	{
		(*iter)->reset();
	}
}

const Quaternion &Node::_getDerivedOrientation(void)
{
	if(m_needSelfUpdate)
	{
		_updateFromParent();
	}
	return m_derivedOrientation;
}

const Vector3f &Node::_getDerivedPosition(void)
{
	if(m_needSelfUpdate)
	{
		_updateFromParent();
	}
	return m_derivedPosition;
}

const Vector3f &Node::_getDerivedScale(void)
{
	if(m_needSelfUpdate)
	{
		_updateFromParent();
	}
	return m_derivedScale;
}

const Matrix4f& Node::_getFullTransform(void)
{
	//矩阵运算计算量较大，使用下面的条件以避免无谓的计算
	if (m_cachedTransformOutOfDate)
	{
		m_cachedTransform.makeTransformMatrix(_getDerivedPosition(), _getDerivedScale(), _getDerivedOrientation());
		m_needSelfUpdate = false;
		m_cachedTransformOutOfDate = false;
	}
	return m_cachedTransform;
}

void Node::_needUpdate(bool forceParentUpdate)
{
	m_needSelfUpdate = true;
	m_needChildUpdate = true;
	m_cachedTransformOutOfDate = true;
	
	// Make sure we're not root and parent hasn't been notified before
        if (mp_parent && (!m_parentNotified || forceParentUpdate))
        {
		mp_parent->_requestUpdate(this, forceParentUpdate);
		m_parentNotified = true ;
        }

        // all children will be updated
        m_childUpdateVec.clear();
}

void Node::_requestUpdate(Node* child, bool forceParentUpdate)
{        
	// If we're already going to update everything this doesn't matter
	if (m_needChildUpdate)
	{
		return;
	}

	m_childUpdateVec.push_back(child);
        // Request selective update of me, if we didn't do it before
        if (mp_parent && (!m_parentNotified || forceParentUpdate))
	{
           	mp_parent->_requestUpdate(this, forceParentUpdate);
		m_parentNotified = true ;/*防止多次通知父节点*/
	}
}

void Node::_update(bool updateChildren, bool parentHasChanged)
{
	
	// always clear information about parent notification
	m_parentNotified = false;

	// See if we should process everyone
	if (m_needSelfUpdate || parentHasChanged)
	{
		// Update transforms from parent
		_updateFromParent();
	}

	if(updateChildren)
	{
		if (m_needChildUpdate || parentHasChanged)
		{
			for(ChildNodeIterator iter = m_childVec.begin(); iter != m_childVec.end(); ++iter)
			{
				(*iter)->_update(true, true);
			}
		}
		else
		{
			// Just update selected children
			ChildNodeIterator it, itend;
			itend =m_childUpdateVec.end();
			for(it = m_childUpdateVec.begin(); it != itend; ++it)
			{
				Node* child = *it;
				child->_update(true, false);
			}
		}
		
		m_childUpdateVec.clear();
		m_needChildUpdate = false;
	}
}

void Node::_updateFromParent(void)
{
	_updateFromParentImpl();
}

void Node::_updateFromParentImpl(void)
{
	if (mp_parent)
	{
		// Update orientation
		const Quaternion& parentOrientation = mp_parent->_getDerivedOrientation();
		// Combine orientation with that of parent
		m_derivedOrientation = parentOrientation * m_orientation;
		// Update scale
		const Vector3f& parentScale = mp_parent->_getDerivedScale();
		// Scale own position by parent scale, NB just combine
		// as equivalent axes, no shearing
		m_derivedScale = parentScale * m_scale;
		// Change position vector based on parent's orientation & scale
		m_derivedPosition = (parentScale * m_position) * parentOrientation;
		// Add altered position vector to parents
		m_derivedPosition += mp_parent->_getDerivedPosition();
	}
	else
	{
		// Root node, no parent
		m_derivedOrientation = m_orientation;
		m_derivedPosition = m_position;
		m_derivedScale = m_scale;
	}
	m_cachedTransformOutOfDate = true;
	m_needSelfUpdate = false;
}

void Node::addChild(Node *child)
{
	if(child->getParent())
	{
		fprintf(stderr, "Node::AddChild: child already has a parent, ADD FAILURE\n");
		return;
	}
	m_childVec.push_back(child);
	child->setParent(this);
}

Node* Node::createChild(const std::string& name)
{
	Node* newNode = _createChildImpl(name);
	this->addChild(newNode);
	newNode->_needUpdate();
	return newNode;
}

Node *Node::_createChildImpl(const  std::string& name)
{
	Node *newNode = new Node(name);
	return newNode;
}

void Node::detachAllChildren()
{
	//因为所有的SceneNode都是用户创建的
	//因此不能在这里进行销毁工作，由用户自己处理
	for(ChildNodeIterator it = m_childUpdateVec.begin(); it != m_childUpdateVec.end(); ++ it)
	{
		(*it)->setParent(NULL);
	}
}

void Node::detachChild(Node *child)
{
	if (child)
	{
		for(ChildNodeIterator iter = m_childVec.begin(); iter != m_childVec.end(); ++iter)
		{
			if((*iter) == child)
			{
				m_childVec.erase(iter);
				break;
			}
		}
	}
}

unsigned short Node::numChildren(void) const
{
	return static_cast<unsigned short>(m_childVec.size());
}

Node *Node::getChild(unsigned short index) const
{
	if( index < m_childVec.size() )
		return m_childVec[index];
	else
		return NULL;
}

Node *Node::getChild(const std::string& name) const
{
	Node *p_childNode;
	for(ConstChildNodeIterator iter = m_childVec.begin(); iter != m_childVec.end(); ++iter)
	{
		if((*iter)->getName() == name) return *iter;
		else if((p_childNode = (*iter)->getChild(name)) != NULL )  return p_childNode;
	}
	return NULL;
}

const Quaternion &Node::getOrientation(void) const
{
	return m_orientation;
}

void Node::setOrientation(const Quaternion &q)
{
	m_orientation = q;
	m_orientation.normalise();
	_needUpdate();
	return;
}

void Node::setOrientation(float w, float x, float y, float z)
{
	setOrientation(Quaternion(x,y,z,w));
	return;
}

void Node::resetOrientation(void)
{
	m_orientation = Quaternion(0,0,0,1);
	_needUpdate();
	return;
}

const Vector3f &Node::getPosition(void)
{
	return m_position;
}

void Node::setPosition(const Vector3f &v)
{
	m_position = v;
	_needUpdate();
	return;
}

void Node::setPosition(float x, float y, float z)
{
	setPosition(Vector3f(x,y,z));
	return;
}

const Vector3f &Node::getScale(void)
{
	return m_scale;
}

void Node::setScale(const Vector3f &v)
{
	m_scale = v;
	_needUpdate();
	return;
}

void Node::setScale(float x, float y, float z)
{
	setScale(Vector3f(x,y,z));
	return;
}

void Node::translate(const Vector3f &d, TransformSpace relativeTo)
{
	switch(relativeTo)
	{
	case TS_LOCAL:
		// position is relative to parent so transform downwards
		m_position += d * m_orientation;
		break;
	case TS_WORLD:
		// position is relative to parent so transform upwards
		if (mp_parent)
		{
		m_position += (d * mp_parent->_getDerivedOrientation().inverse())
		/ mp_parent->_getDerivedScale();
		}
		else
		{
		m_position += d;
		}
		break;
	case TS_PARENT:
		m_position += d;
	break;
	}
    _needUpdate();
}

void Node::translate(float x, float y, float z, TransformSpace relativeTo)
{
	translate(Vector3f(x,y,z),relativeTo);
	return;
}

void Node::rotate(const Quaternion &q, TransformSpace relativeTo)
{
	// Normalise quaternion to avoid drift
	Quaternion qnorm = q;
	qnorm.normalise();

	switch(relativeTo)
	{
	case TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		m_orientation = qnorm * m_orientation;
		break;
	case TS_WORLD:
		// Rotations are normally relative to local axes, transform up
		m_orientation = m_orientation * _getDerivedOrientation().inverse()
		* qnorm * _getDerivedOrientation();
		break;
	case TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		m_orientation = m_orientation * qnorm;
	break;
	}
	_needUpdate();
}

void Node::rotate(const Vector3f &axis, float angle, TransformSpace relativeTo)
{
	rotate(Quaternion(axis, angle), relativeTo);
	return;
}

void Node::roll(float angle, TransformSpace relativeTo)
{
	rotate(Vector3f(0.0f, 0.0f, 1.0f),angle, relativeTo);
	return;
}

void Node::pitch(float angle, TransformSpace relativeTo)
{
	rotate(Vector3f(1.0f, 0.0f, 0.0f), angle, relativeTo);
}

void Node::yaw(float angle, TransformSpace relativeTo)
{
	rotate(Vector3f(0.0f,1.0f,0.0f),angle,relativeTo);
}

void Node::scale(const Vector3f &v)
{
	m_scale *= v;
	_needUpdate();
}

void Node::scale(float x, float y, float z)
{
	scale(Vector3f(x, y, z));
}

void Node::setInitialState(void)
{
	m_initialPosition = m_position;
	m_initialOrientation = m_orientation;
	m_initialScale = m_scale;
}

void Node::resetToInitialState(void)
{
	m_position = m_initialPosition;
	m_orientation = m_initialOrientation;
	m_scale = m_initialScale;

	_needUpdate();
}
}
