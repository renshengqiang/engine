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
	AnimationState�������:�Զ�����״̬���й���
	������״̬:
	1. �������ͳ�����Ϣ(Animation��Ҳ�б��棬��ʵ���Ǵ�Animation�и��ƹ�����)
	2. ��ǰ��ʱ��λ��
	3. �����Ƿ�ʹ��
	4. �����Ƿ�ѭ������
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

/*AnimationStateSet����ͳһ����AnimationState*/
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
