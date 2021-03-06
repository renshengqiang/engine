#ifndef _ENGINE_ANIMATION_TRACK_H
#define _ENGINE_ANIMATION_TRACK_H
#include <vector>
#include <string>

namespace Engine
{
class Animation;
class Node;
class KeyFrame;
class TransformKeyFrame;
/*
	AnimationTrack类的作用:
	1. 对Track中的时间相关的操作进行封装；
	2. 为了适应更多类型的和节点相关的动画，将此类抽象出来作为多种节点相关动画类的父类
*/
class AnimationTrack
{
public:
	typedef std::vector<KeyFrame*> KeyFrameList;
	typedef KeyFrameList::iterator KeyFrameIterator;
	typedef KeyFrameList::reverse_iterator KeyFrameReverseIterator;
	
	AnimationTrack(Animation *parent);
	virtual ~AnimationTrack();

	
	unsigned short numKeyFrames(void) const
	{
		return m_keyFrameList.size();
	}
	
	KeyFrame *getKeyFrame(unsigned short index)
	{
		if(index < m_keyFrameList.size())
		{
			return m_keyFrameList[index];
		}
		return NULL;
	}
	
	/**	根据给定的时间创建一个KeyFrame,保存在m_keyFrameArray数组中
		m_keyFrameArray数组中的keyFrame是根据他们的timePos来升序排序的
	*/
	KeyFrame *createKeyFrame(float timePos);
	virtual void removeKeyFrame(unsigned short index);
	virtual void removeAllKeyFrames(void);
	
	/*
		获取指定timePos处的两个KeyFrame,并返回两个KeyFrame之间的混合值(即比例)
	@remarks
		在任意时刻总对应着动画中的1帧或者2帧:1 timePos正好对应某个keyFrame的时间，2 除1之外其他情况，
		两帧分别对应timePos前的一帧和timePos后面的一帧
		该函数返回任意时间时刻对应的关键帧，同时返回一个参数表示返回的两个帧之间的插值比例，
		例如:如果返回0表示，该时间正好对应keyFrame1,0.5则表示在kerFrame1和keyFrame2之间,即返回的数表示keyFrame2所占的比例
		
	*/
	virtual float getKeyFrameAtTime(float timePos, KeyFrame **keyFrame1, KeyFrame **keyFrame2, int *index);

	/*
		获取timePos时间处的KeyFrame信息，该KeyFrame中保存了timePos处应该保存的trans信息
	@param
		timePos:指定某时间点处的动画
		kf:得到的KeyFrame的存放位置
	*/
	virtual void getInterpolatedKeyFrame(float timePos, KeyFrame *kf) = 0;
	virtual void apply(float timePos, float weight = 1.0, float scale = 1.0) = 0;
	Animation *getParent() const { return mp_parent;}
	
protected:
	Animation *mp_parent;
	KeyFrameList m_keyFrameList;
	
	virtual KeyFrame *_createKeyFrameImpl(float timePos) = 0;
	virtual void _keyFrameDataChanged(void) const {}
};
/*
	NodeAnimationTrack类的作用: 对属于NodeAnimation的属性(位移，旋转，缩放)进行封装和管理
*/
class NodeAnimationTrack : public AnimationTrack
{
public:	
	NodeAnimationTrack(Animation *parent, const std::string &name="");
	NodeAnimationTrack(Animation *parent, Node *targetNode, const std::string &name="");
	~NodeAnimationTrack();

	
	Node *getAssociatedNode(void) const;
	void setAssociatedNode(Node *node);
	
	bool getUseShortestPath(void) const;
	void setUseShortestPath(bool useShortestPath);
	
	TransformKeyFrame *createNodeKeyFrame(float timePos);
	virtual void getInterpolatedKeyFrame(float timePos, KeyFrame *kf);
	
	virtual void apply(float timePos,float weight = 1.0,float scale = 1.0);
	void applyToNode(Node *node, float timePos, float weight=1.0, float scale=1.0);

	
private:
	std::string m_name;
	Node *mp_targetNode;

	// Prebuilt splines, must be mutable since lazy-update in const method
	//mutable Splines* mp_splines;
	mutable bool m_useShortestRotationPath;
	mutable bool m_splineBuildNeeded;

	virtual KeyFrame *_createKeyFrameImpl(float timePos);
	virtual void _keyFrameDataChanged(void) const;
};
}

#endif
