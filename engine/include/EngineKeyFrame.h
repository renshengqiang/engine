#ifndef _ENGINE_KEYFRAME_H
#define _ENGINE_KEYFRAME_H
#include <EngineVector.h>
#include <EngineQuaternion.h>

namespace Engine
{
class AnimationTrack;
class KeyFrame
{
	public:
		KeyFrame(AnimationTrack *track, float time):
			mp_parentTrack(track), m_time(time)
		{
		}
		float getTime(void) const { return m_time;}
		AnimationTrack *getAnimationTrack() { return mp_parentTrack;}

	protected:
		AnimationTrack *mp_parentTrack;
		float m_time;
};

class TransformKeyFrame: public KeyFrame
{
	public:
		TransformKeyFrame(AnimationTrack *parent, float time):
			KeyFrame(parent, time)
		{
			m_translate = Vector3f::ZERO;
			m_scale = Vector3f::UNIT_SCALE;
		}

		void setTranslate(const Vector3f &trans)
		{
			m_translate = trans;
		}
		const Vector3f &getTranslate(void) const
		{
			return m_translate;
		}
		void setRotation(const Quaternion &rotate)
		{
			m_rotate = rotate;
		}
		const Quaternion &getRotation(void) const
		{
			return m_rotate;
		}
		void setScale(const Vector3f &scale)
		{
			m_scale  = scale;
		}
		const Vector3f &getScale(void) const
		{
			return m_scale;
		}
		
	protected:
		Vector3f m_translate;
		Vector3f m_scale;
		Quaternion m_rotate;
};
}

#endif

