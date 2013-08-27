#include "EngineAnimationTrack.h"
#include "EngineAnimation.h"
#include "EngineNode.h"
#include "EngineKeyFrame.h"
#include <stdio.h>

namespace Engine
{
AnimationTrack::AnimationTrack(Animation * parent)
	:mp_parent(parent)
{
}

AnimationTrack::~AnimationTrack()
{
	removeAllKeyFrames();
}

KeyFrame *AnimationTrack::createKeyFrame(float timePos)
{
	KeyFrame *kf = _createKeyFrameImpl(timePos);
	KeyFrameReverseIterator iter;
	
	//采用插入排序的方法
	m_keyFrameList.push_back(kf);
	for(iter = m_keyFrameList.rbegin() +1; iter != m_keyFrameList.rend(); ++iter)
	{
		if((*iter)->getTime() > timePos)
		{
			*(iter - 1) = *iter;
		}
		else
		{
			--iter;//就是插在iter这个位置
			break;
		}
	}
	if(iter == m_keyFrameList.rend())
	{
		m_keyFrameList[0] = kf;
	}
	else
	{
		*iter = kf;
	}
	_keyFrameDataChanged();
	return kf;	
}

void AnimationTrack::removeKeyFrame(unsigned short index)
{
	if(index < m_keyFrameList.size())
	{
		KeyFrameIterator iter = m_keyFrameList.begin();
		while(index--) ++iter;
		delete *iter;
		m_keyFrameList.erase(iter);
	}
	_keyFrameDataChanged();
	return;
}

void AnimationTrack::removeAllKeyFrames(void)
{
	for(KeyFrameIterator iter = m_keyFrameList.begin(); iter != m_keyFrameList.end(); ++iter)
	{
		KeyFrame *pKeyFrame = *iter;
		delete pKeyFrame;
	}
	m_keyFrameList.clear();
	_keyFrameDataChanged();
	return;
}

/**	timePos: 当前动画时间
	keyFrame1: 当前时间点前面一帧
	keyFrame2: 当前时间点后面一帧
	index: keyFrame1的index
	return value: 当前获得的第二帧占据的比重
	
	按两种情况进行分别处理:
	case 1. 当前的时间在最后一帧的后面或者就是最后一帧，简单使用最后一帧作为输出
	case 2. 正常情况，在正常的两帧之间
*/
float AnimationTrack::getKeyFrameAtTime(float timePos, KeyFrame **keyFrame1, KeyFrame **keyFrame2, int *index)
{
	float totalAnimationLength = mp_parent->getLength();
	float t1,t2;
	unsigned i; 
	
	assert(totalAnimationLength > 0.0f && "Invalid animation length!");

	if(timePos > totalAnimationLength)
		timePos = fmodf(timePos, totalAnimationLength);
	
	for(i=0; i<m_keyFrameList.size(); ++i)
	{
		if(m_keyFrameList[i]->getTime() > timePos) break;
	}
	if(i == m_keyFrameList.size())//case 1
	{
		if(m_keyFrameList.size() > 0)
		{
			*keyFrame1 = m_keyFrameList[m_keyFrameList.size() - 1];
			*keyFrame2 = m_keyFrameList[0];
			*index = m_keyFrameList.size() - 1;
		}
		else
		{
			*keyFrame1 = *keyFrame2 = NULL;
			*index = 0;
		}
		return 0;
	}
	else//case 2
	{
		*keyFrame2 = m_keyFrameList[i];
		t2=(*keyFrame2)->getTime();
		if(i!=0)--i;//防止第一帧不存在的情况，一般不会出现
		*keyFrame1 = m_keyFrameList[i];
		t1=(*keyFrame1)->getTime();
		*index = i;

		if(t1==t2) return timePos/t2;
		else return (timePos-t1)/(t2-t1);
	}
}





NodeAnimationTrack::NodeAnimationTrack(Animation *parent, const std::string &name)
	:AnimationTrack(parent),
	m_name(name),
	mp_targetNode(0),
	m_useShortestRotationPath(false),
	m_splineBuildNeeded(false)
{
}

NodeAnimationTrack::NodeAnimationTrack(Animation *parent, Node *targetNode, const std::string &name)
	:AnimationTrack(parent),
	m_name(name),
	mp_targetNode(targetNode),
	m_useShortestRotationPath(false),
	m_splineBuildNeeded(false)
{
}

NodeAnimationTrack::~NodeAnimationTrack()
{
}

Node *NodeAnimationTrack::getAssociatedNode(void) const
{
	return mp_targetNode;
}

void NodeAnimationTrack::setAssociatedNode(Node *node)
{
	mp_targetNode = node;
}

bool NodeAnimationTrack::getUseShortestPath(void) const
{
	return m_useShortestRotationPath;
}

void NodeAnimationTrack::setUseShortestPath(bool useShortestPath)
{
	m_useShortestRotationPath = useShortestPath;
}

TransformKeyFrame *NodeAnimationTrack::createNodeKeyFrame(float timePos)
{
	return static_cast<TransformKeyFrame*>(createKeyFrame(timePos));
}

KeyFrame *NodeAnimationTrack::_createKeyFrameImpl(float timePos)
{
	TransformKeyFrame *keyFrame = new TransformKeyFrame(this,timePos);
	return keyFrame;
}	

void NodeAnimationTrack::_keyFrameDataChanged(void) const
{
	m_splineBuildNeeded = true;
}

/*调用getKeyFrameAtTime得到两个KeyFrame,并根据这两个keyFrame进行插值*/
void NodeAnimationTrack::getInterpolatedKeyFrame(float timePos, KeyFrame *kf)
{
	TransformKeyFrame *keyFrameRet = static_cast<TransformKeyFrame*>(kf);
	KeyFrame *kBase1,*kBase2;
	int index;
	float rate2 = getKeyFrameAtTime(timePos,&kBase1,&kBase2, &index);

	if(kBase1 && kBase2)
	{
		TransformKeyFrame *k1 = static_cast<TransformKeyFrame*>(kBase1);
		TransformKeyFrame *k2 = static_cast<TransformKeyFrame*>(kBase2);

		Quaternion k1Quaternion(0,0,0,1);
		Vector3f k1Translate(0,0,0);
		Vector3f k1Scale(1,1,1);
		if(k1!=k2)
		{
			k1Quaternion = k1->getRotation();
			k1Translate = k1->getTranslate();
			k1Scale = k1->getScale();
		}
		
		Animation::InterpolationMode im = mp_parent->getInterpolationMode();
		Animation::RotationInterpolationMode rim = mp_parent->getRotationInterpolationMode();
		switch(im)
		{
			case Animation::IM_LINEAR:
				if(rim == Animation::RIM_LINEAR)
				{
					keyFrameRet->setRotation(Quaternion::nlerp(rate2, k1Quaternion, k2->getRotation(), m_useShortestRotationPath));
				}
				else
				{
					keyFrameRet->setRotation(Quaternion::slerp(rate2, k1Quaternion, k2->getRotation(), m_useShortestRotationPath));
				}
				keyFrameRet->setTranslate(k1Translate*(1-rate2) + k2->getTranslate()*rate2);
				keyFrameRet->setScale(k1Scale*(1-rate2) + k2->getScale()*rate2);
				break;
			/*
			case Animation::IM_SPLINE:
				if(m_splineBuildNeeded)
				{
					_buildInterpolationSplines();
				}
				// Rotation, take mUseShortestRotationPath into account
				keyFrameRet->setRotation( mp_splines->rotationSpline.interpolate(index, rate2,
				m_useShortestRotationPath));

				// Translation
				keyFrameRet->setTranslate( mp_splines->positionSpline.interpolate(index, rate2) );

				// Scale
				keyFrameRet->setScale( mp_splines->scaleSpline.interpolate(index, rate2) );
				break;
			*/
			default:
				break;
		}
	}
	else
	{
		keyFrameRet->setTranslate(Vector3f(0.0, 0.0, 0.0));
		keyFrameRet->setScale(Vector3f(1.0, 1.0, 1.0));
		keyFrameRet->setRotation(Quaternion(0.0, 0.0, 0.0, 1.0));
	}
}

void NodeAnimationTrack::apply(float timePos,float weight,float scale)
{
	applyToNode(mp_targetNode,timePos,weight,scale);
}

void NodeAnimationTrack::applyToNode(Node *node, float timePos, float weight, float scl)
{
	TransformKeyFrame kf(0, timePos);

	getInterpolatedKeyFrame(timePos, &kf);

	Vector3f translate = kf.getTranslate()*(weight*scl);
	node->translate(translate);
	
	Quaternion rotate;
	Animation::RotationInterpolationMode rim = mp_parent->getRotationInterpolationMode();
	if(rim == Animation::RIM_LINEAR)
	{
		rotate = Quaternion::nlerp(weight, Quaternion(0,0,0,1), kf.getRotation(), m_useShortestRotationPath);
	}
	else
	{
		rotate = Quaternion::slerp(weight, Quaternion(0,0,0,1), kf.getRotation(), m_useShortestRotationPath);
	}
	node->rotate(rotate);
	
	Vector3f scale = kf.getScale();
	if(!(scale==Vector3f(1,1,1)))
	{
		if(scl != 1.0f)
			scale = Vector3f(1,1,1) + (scale - Vector3f(1,1,1)) * scl;
		else if(weight!=1.0f)
			scale = Vector3f(1,1,1) + (scale - Vector3f(1,1,1)) * weight;
	}
	node->scale(scale);
	return;
}
}
