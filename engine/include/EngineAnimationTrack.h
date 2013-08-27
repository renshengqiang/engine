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
	AnimationTrack�������:
	1. ��Track�е�ʱ����صĲ������з�װ��
	2. Ϊ����Ӧ�������͵ĺͽڵ���صĶ�������������������Ϊ���ֽڵ���ض�����ĸ���
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
	
	/**	���ݸ�����ʱ�䴴��һ��KeyFrame,������m_keyFrameArray������
		m_keyFrameArray�����е�keyFrame�Ǹ������ǵ�timePos�����������
	*/
	KeyFrame *createKeyFrame(float timePos);
	virtual void removeKeyFrame(unsigned short index);
	virtual void removeAllKeyFrames(void);
	
	/*
		��ȡָ��timePos��������KeyFrame,����������KeyFrame֮��Ļ��ֵ(������)
	@remarks
		������ʱ���ܶ�Ӧ�Ŷ����е�1֡����2֡:1 timePos���ö�Ӧĳ��keyFrame��ʱ�䣬2 ��1֮�����������
		��֡�ֱ��ӦtimePosǰ��һ֡��timePos�����һ֡
		�ú�����������ʱ��ʱ�̶�Ӧ�Ĺؼ�֡��ͬʱ����һ��������ʾ���ص�����֮֡��Ĳ�ֵ������
		����:�������0��ʾ����ʱ�����ö�ӦkeyFrame1,0.5���ʾ��kerFrame1��keyFrame2֮��,�����ص�����ʾkeyFrame2��ռ�ı���
		
	*/
	virtual float getKeyFrameAtTime(float timePos, KeyFrame **keyFrame1, KeyFrame **keyFrame2, int *index);

	/*
		��ȡtimePosʱ�䴦��KeyFrame��Ϣ����KeyFrame�б�����timePos��Ӧ�ñ����trans��Ϣ
	@param
		timePos:ָ��ĳʱ��㴦�Ķ���
		kf:�õ���KeyFrame�Ĵ��λ��
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
	NodeAnimationTrack�������: ������NodeAnimation������(λ�ƣ���ת������)���з�װ�͹���
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
