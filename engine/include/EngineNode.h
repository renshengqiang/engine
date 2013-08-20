#ifndef _ENGINE_NODE_H
#define _ENGINE_NODE_H

#include <EngineVector.h>
#include <EngineMatrix.h>
#include <EngineQuaternion.h>
#include <string>
#include <vector>

namespace Engine
{
/**  Node类主要负责关于空间位置变换有关参数和函数
	继承者不需要再处理空间变换的数学计算
*/
class Node
{
public:
	//用于指定变换相对谁的枚举变量
	enum TransformSpace
	{
		/// Transform is relative to the local space
		TS_LOCAL,
		/// Transform is relative to the space of the parent node
		TS_PARENT,
		/// Transform is relative to world space
		TS_WORLD
	};
	//typedef std::map<std::string, Node *> ChildrenMap;
	//typedef std::map<std::string, Node *>::iterator ChildIterator;
	typedef std::vector<Node *> ChildNodeVector;
	typedef std::vector<Node *>::iterator ChildNodeIterator;
	typedef std::vector<Node *>::const_iterator ConstChildNodeIterator;
	
	Node();
	Node(const std::string &name);
	virtual ~Node();
	
	inline const std::string& getName(void) const
	{
		return m_name;
	}
	inline Node *getParent(void) const
	{
		return mp_parent;
	}
	inline void setParent(Node *parent)
	{
		if(mp_parent != parent)
		{
			mp_parent = parent;
			_needUpdate();
		}
	}
	//复位到原始状态
	void reset(void);

	//获取该node相对于世界的方向
	const Quaternion& _getDerivedOrientation(void);
	
	//获取该node相对于世界的位置
	const Vector3f& _getDerivedPosition(void);
	
	//获取该node相对于世界的缩放
	const Vector3f& _getDerivedScale(void);
	
	/**
		获取该node的full transformation matrix
		即它的所有父node的节点变换的总和以及自身的变换的和
		在获取之前需要先嗲用Node::_update进行更新，保证所有的最新变换都反映到变换矩阵中
		一般是SceneManager调用的，先update，然后_getFullTransform
		应用程序一般需要使用相对的变换而不是全局的变换
	*/
	const Matrix4f& _getFullTransform(void);
	
	//获取该node相对于父亲的方向
	const Quaternion& getOrientation(void) const;
	
	//获取该node相对于父节点的位置	
	const Vector3f& getPosition(void);
	
	//获取该node相对于父节点的缩放	
	const Vector3f& getScale(void);

	//功能函数，记录需要update的标识
	virtual void _needUpdate(bool forceParentUpdate = false);

	//添加一个child node
	void addChild(Node *child);

	/**	create a child node
		这里使用了strategy模式:内部使用_createChildImpl虚函数来创建不同的节点
	*/
	Node *createChild(const std::string &name);

	//删除一个child node
	void removeChild(Node *child);
	
	unsigned short numChildren(void) const;
	Node *getChild(unsigned short index) const;
	Node *getChild(const std::string &name) const;
	
	void setOrientation(const Quaternion &q);
	void setOrientation(float w, float x, float y, float z);
	void resetOrientation(void);
	void setPosition(const Vector3f &v);
	void setPosition(float x, float y, float z);
	void setScale(const Vector3f &v);
	void setScale(float x, float y, float z);
	
	void translate(const Vector3f &d, TransformSpace relativeTo=TS_PARENT);
	void translate(float x, float y, float z, TransformSpace relativeTo=TS_PARENT);
	void rotate(const Quaternion &q, TransformSpace relativeTo=TS_PARENT);
	void rotate(const Vector3f &axis, float angle, TransformSpace relativeTo=TS_PARENT);
	void roll(float angle, TransformSpace relativeTo=TS_PARENT);
	void pitch(float angle, TransformSpace relativeTo=TS_PARENT);
	void yaw(float angle, TransformSpace relativeTo=TS_PARENT);
	void scale(const Vector3f &v);
	void scale(float x, float y, float z);
	
	/** 将node的当前变换作为该节点的'initial state'
		initial state是动画在节点上进行变换的初始状态

		一般只有在该节点上加上动画后才会使用该函数
		并且是希望该动画是node的当前transform上的基础上进行的

		如果从不调用这个函数，initial state即为identity transform即什么变换都不做
		此时在该节点上作动画，会从parent node位置处进行变换
	*/
	void setInitialState(void);
	
	/**  将initial state变换为当前的state
		一般用于当动画结束后，让动画的变换不停留在该节点上
	*/
	void resetToInitialState(void);
	
protected:
	//更新节点，如果updateChildren为true则递归的完成child nodes的更新
	virtual void _update(bool updateChildren, bool parentHasChanged);
	
	//孩子nodes通知parent自己需要更新，用于在SceneManager中得到渲染队列
	void _requestUpdate(Node* child, bool forceParentUpdate = false);
	
	void _updateFromParent(void);
	virtual void _updateFromParentImpl(void) ;
	/** Internal method for creating a new child node - subclass need overridden this. */
	virtual Node* _createChildImpl(const std::string &name);
	
protected:
	
	std::string m_name;						//节点的名字
	Node *mp_parent;						//父节点
	//ChildrenMap m_childMap;	
	ChildNodeVector m_childVec;				//child nodes
	ChildNodeVector m_childUpdateVec;		//需要更新transform matrix的child nodes
	Quaternion	m_orientation;				//保存了该node相对于parent的方向变换
	Vector3f	m_position;					//保存了该node相对于parent的translation
	Vector3f	m_scale;						//保存了该node相对于parent的scale
	Quaternion	m_initialOrientation;			//initial transform 之方向变换
	Vector3f	m_initialPosition;				//initial transform之位置变换
	Vector3f	m_initialScale;					//initial transform之缩放变换
	Quaternion m_derivedOrientation; 			//保存了该node相对于世界的方向变换
	Vector3f m_derivedPosition;				//保存了该node相对于世界的位置变换
	Vector3f m_derivedScale;				//保存了该node相对于世界的缩放变换
	Matrix4f	m_cachedTransform;			//保存的该节点的transform matrix
	
	mutable bool m_needSelfUpdate;			//当前节点是否需要更新
	mutable bool m_needChildUpdate;			//孩子结点是否需要更新
	mutable bool m_parentNotified;			//是否已经通知父节点需要更新，用于防止多次nitify
	mutable bool m_cachedTransformOutOfDate;	//当前tranform matrix是否需要更新
};
}
#endif
