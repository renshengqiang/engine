#ifndef _ENGINE_ANIMATION_STATE_H
#define _ENGINE_ANIMATION_STATE_H
#include <string>
#include <set>
#include <map>
#include <math.h>

#define EPISITION 0.001F

namespace Engine
{
/*
	AnimationState类的作用:对动画的状态进行管理
	动画的状态:
	1. 动画名和长度信息(Animation中也有保存，其实就是从Animation中复制过来的)
	2. 当前的时间位置
	3. 动画是否使能
	4. 动画是否循环播放
*/
class AnimationStateSet;
class AnimationState
{
	public:
		AnimationState(const std::string &name, float length, 
						AnimationStateSet *parent, 
						float timePos=0.0, bool enabled=false):
			mp_parent(parent),
			m_name(name),
			m_timePos(timePos), 
			m_length(length), 
			m_enabled(enabled)
		{}
		
		const std::string &getName(void) const { return m_name; }
		
		float getTimePosition(void) const { return m_timePos; }
		void setTimePosition(float timePos);
		
		float getLength(void) const { return m_length; }
		void setLength(float length) {  m_length = length; }
		void addTime(float offset) { setTimePosition(m_timePos + offset); }
		
		bool hasEnded(void) const { return (!m_loop && (m_timePos - m_length > EPISITION)); }
		bool getEnabled(void) const { return m_enabled; }
		void setEnabled(bool enabled);
		
		void setLoop(bool loop){ m_loop = loop;};
		bool getLoop(void){ return m_loop;};
		
	private:
		AnimationStateSet *mp_parent;
		std::string m_name;
		float m_timePos;
		float m_length;
		bool m_enabled;
		bool m_loop;
};

/*AnimationStateSet用于统一管理AnimationState*/
class AnimationStateSet
{
	public:
		typedef std::map<std::string, AnimationState*> AnimationStateList;
		typedef AnimationStateList::iterator AnimationStateIterator;
		typedef std::set<AnimationState*> EnabledAnimationStateList;
		typedef EnabledAnimationStateList::iterator EnabledAnimationStateIterator;
		
		AnimationStateSet();
		~AnimationStateSet();
		
		AnimationState *createAnimationState(const std::string &name, float length, float timePos = 0.0, bool enabled = false);
		AnimationState *getAnimationState(const std::string &name);
		bool hasAnimationState(const std::string &name) const;
		void removeAnimationState(const std::string &name);
		void removeAllAnimationStates(void);

		EnabledAnimationStateIterator _animationIteratorBegin(void);
		EnabledAnimationStateIterator _animationIeratorEnd(void);

		void _notifyAnimationStateEnabled(AnimationState *target, bool enabled);
		
	protected:
		AnimationStateList m_animationStateList;
		EnabledAnimationStateList m_enabledAnimationStateList;
};

}

#endif
