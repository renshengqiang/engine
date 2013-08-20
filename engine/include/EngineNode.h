#ifndef _ENGINE_NODE_H
#define _ENGINE_NODE_H

#include <EngineVector.h>
#include <EngineMatrix.h>
#include <EngineQuaternion.h>
#include <string>
#include <vector>

namespace Engine
{
/**  Node����Ҫ������ڿռ�λ�ñ任�йز����ͺ���
	�̳��߲���Ҫ�ٴ���ռ�任����ѧ����
*/
class Node
{
public:
	//����ָ���任���˭��ö�ٱ���
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
	//��λ��ԭʼ״̬
	void reset(void);

	//��ȡ��node���������ķ���
	const Quaternion& _getDerivedOrientation(void);
	
	//��ȡ��node����������λ��
	const Vector3f& _getDerivedPosition(void);
	
	//��ȡ��node��������������
	const Vector3f& _getDerivedScale(void);
	
	/**
		��ȡ��node��full transformation matrix
		���������и�node�Ľڵ�任���ܺ��Լ�����ı任�ĺ�
		�ڻ�ȡ֮ǰ��Ҫ������Node::_update���и��£���֤���е����±任����ӳ���任������
		һ����SceneManager���õģ���update��Ȼ��_getFullTransform
		Ӧ�ó���һ����Ҫʹ����Եı任������ȫ�ֵı任
	*/
	const Matrix4f& _getFullTransform(void);
	
	//��ȡ��node����ڸ��׵ķ���
	const Quaternion& getOrientation(void) const;
	
	//��ȡ��node����ڸ��ڵ��λ��	
	const Vector3f& getPosition(void);
	
	//��ȡ��node����ڸ��ڵ������	
	const Vector3f& getScale(void);

	//���ܺ�������¼��Ҫupdate�ı�ʶ
	virtual void _needUpdate(bool forceParentUpdate = false);

	//���һ��child node
	void addChild(Node *child);

	/**	create a child node
		����ʹ����strategyģʽ:�ڲ�ʹ��_createChildImpl�麯����������ͬ�Ľڵ�
	*/
	Node *createChild(const std::string &name);

	//ɾ��һ��child node
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
	
	/** ��node�ĵ�ǰ�任��Ϊ�ýڵ��'initial state'
		initial state�Ƕ����ڽڵ��Ͻ��б任�ĳ�ʼ״̬

		һ��ֻ���ڸýڵ��ϼ��϶�����Ż�ʹ�øú���
		������ϣ���ö�����node�ĵ�ǰtransform�ϵĻ����Ͻ��е�

		����Ӳ��������������initial state��Ϊidentity transform��ʲô�任������
		��ʱ�ڸýڵ��������������parent nodeλ�ô����б任
	*/
	void setInitialState(void);
	
	/**  ��initial state�任Ϊ��ǰ��state
		һ�����ڵ������������ö����ı任��ͣ���ڸýڵ���
	*/
	void resetToInitialState(void);
	
protected:
	//���½ڵ㣬���updateChildrenΪtrue��ݹ�����child nodes�ĸ���
	virtual void _update(bool updateChildren, bool parentHasChanged);
	
	//����nodes֪ͨparent�Լ���Ҫ���£�������SceneManager�еõ���Ⱦ����
	void _requestUpdate(Node* child, bool forceParentUpdate = false);
	
	void _updateFromParent(void);
	virtual void _updateFromParentImpl(void) ;
	/** Internal method for creating a new child node - subclass need overridden this. */
	virtual Node* _createChildImpl(const std::string &name);
	
protected:
	
	std::string m_name;						//�ڵ������
	Node *mp_parent;						//���ڵ�
	//ChildrenMap m_childMap;	
	ChildNodeVector m_childVec;				//child nodes
	ChildNodeVector m_childUpdateVec;		//��Ҫ����transform matrix��child nodes
	Quaternion	m_orientation;				//�����˸�node�����parent�ķ���任
	Vector3f	m_position;					//�����˸�node�����parent��translation
	Vector3f	m_scale;						//�����˸�node�����parent��scale
	Quaternion	m_initialOrientation;			//initial transform ֮����任
	Vector3f	m_initialPosition;				//initial transform֮λ�ñ任
	Vector3f	m_initialScale;					//initial transform֮���ű任
	Quaternion m_derivedOrientation; 			//�����˸�node���������ķ���任
	Vector3f m_derivedPosition;				//�����˸�node����������λ�ñ任
	Vector3f m_derivedScale;				//�����˸�node�������������ű任
	Matrix4f	m_cachedTransform;			//����ĸýڵ��transform matrix
	
	mutable bool m_needSelfUpdate;			//��ǰ�ڵ��Ƿ���Ҫ����
	mutable bool m_needChildUpdate;			//���ӽ���Ƿ���Ҫ����
	mutable bool m_parentNotified;			//�Ƿ��Ѿ�֪ͨ���ڵ���Ҫ���£����ڷ�ֹ���nitify
	mutable bool m_cachedTransformOutOfDate;	//��ǰtranform matrix�Ƿ���Ҫ����
};
}
#endif
