#include "EngineAnimationState.h"
#include <stdio.h>

namespace Engine
{
void AnimationState::setTimePosition(float timePos)
{	
	if (timePos != m_timePos)
	{
		m_timePos = timePos;
		if (m_loop)
		{
			// Wrap
			m_timePos = fmodf(m_timePos, m_length);
			if(m_timePos < 0)
				m_timePos += m_length;     
		}
		else
		{
			// Clamp
			if(m_timePos < 0)
				m_timePos = 0;
			else if (m_timePos > m_length)
			{
				m_timePos = m_length;
				mp_parent->_notifyAnimationStateEnabled(this, false);			
				m_enabled = false;
			}
		}
    }
}

void AnimationState::setEnabled(bool enabled)
{
	if(mp_parent)
		mp_parent->_notifyAnimationStateEnabled(this,enabled);
	m_enabled = enabled;
}




AnimationStateSet::AnimationStateSet()
{
}

AnimationStateSet::~AnimationStateSet()
{
	removeAllAnimationStates();
}

AnimationState *AnimationStateSet::createAnimationState(const std::string &name, 
										float length, float timePos, bool enabled)
{
	if(hasAnimationState(name))
	{
		fprintf(stderr, "ERROR AnimationStateSet::CreateAnimationState : State for animation named %s already exist\n", name.c_str());
		return NULL;
	}
	AnimationState *newState = new AnimationState(name, length, this, timePos, enabled);
	m_animationStateList.insert(std::make_pair(name, newState));
	if(enabled) m_enabledAnimationStateList.insert(newState);
	return newState;
}

AnimationState *AnimationStateSet::getAnimationState(const std::string &name)
{
	AnimationStateIterator iter = m_animationStateList.find(name);
	if(iter != m_animationStateList.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

bool AnimationStateSet::hasAnimationState(const std::string &name) const
{
	return m_animationStateList.count(name) > 0;
}

void AnimationStateSet::removeAnimationState(const std::string &name)
{
	AnimationStateIterator iter = m_animationStateList.find(name);
	if(iter != m_animationStateList.end())
	{
		AnimationState *pState = iter->second;
		if(pState->getEnabled())
		{
			EnabledAnimationStateIterator iter1 = m_enabledAnimationStateList.find(pState);
			if(iter1 != m_enabledAnimationStateList.end())
			{
				m_enabledAnimationStateList.erase(iter1);
			}
		}
		delete pState;
		m_animationStateList.erase(iter);
	}
	return;
}

void AnimationStateSet::removeAllAnimationStates(void)
{
	for(AnimationStateIterator iter = m_animationStateList.begin();
		iter != m_animationStateList.end(); ++iter)
	{
		delete iter->second;
	}
	m_animationStateList.clear();
	m_enabledAnimationStateList.clear();
	return;
}

AnimationStateSet::EnabledAnimationStateIterator 
AnimationStateSet::_animationIteratorBegin(void)
{
	return m_enabledAnimationStateList.begin();
}

AnimationStateSet::EnabledAnimationStateIterator 
AnimationStateSet::_animationIeratorEnd(void)
{
	return m_enabledAnimationStateList.end();
}

void AnimationStateSet::_notifyAnimationStateEnabled(AnimationState *target, bool enabled)
{
	if(target==NULL) return;
	
	if(target->getEnabled() != enabled)
	{
		if(enabled)
		{
			m_enabledAnimationStateList.insert(target);
		}
		else
		{
			EnabledAnimationStateIterator iter = m_enabledAnimationStateList.find(target);
			if(iter != m_enabledAnimationStateList.end())
			{
				m_enabledAnimationStateList.erase(iter);
			}
		}
	}
	return;
}

}
