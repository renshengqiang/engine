#ifndef _ENGINE_RAY_H
#define _ENGINE_RAY_H
#include <EngineVector.h>

namespace Engine
{
class AxisAlignedBox;

//���߾���һ��������һ����������
class Ray
{
public:
	Ray():m_origin(Vector3f::ZERO), m_direction(Vector3f::UNIT_Z) {}
	Ray(const Vector3f& origin, const Vector3f& direction)
		:m_origin(origin), m_direction(direction) {}

	/** Sets the origin of the ray. */
	void setOrigin(const Vector3f& origin) {m_origin = origin;} 
	/** Gets the origin of the ray. */
	const Vector3f& getOrigin(void) const {return m_origin;} 

	/** Sets the direction of the ray. */
	void setDirection(const Vector3f& dir) {m_direction = dir;} 
	/** Gets the direction of the ray. */
	const Vector3f& getDirection(void) const {return m_direction;} 

	/** Gets the position of a point t units along the ray. */
	Vector3f getPoint(float t) const 
	{ 
		return Vector3f(m_origin + (m_direction * t));
	}
	
	/** Gets the position of a point t units along the ray. */
	Vector3f operator*(float t) const 
	{ 
		return getPoint(t);
	}

	/** �жϸù����Ƿ��һ����Χ�н���
		����һ��pair: 
		pair.first��ʾ�Ƿ񽻲�
		pair.second��ʾ������棬�򽻲����������ʼ��ľ���
	*/
	std::pair<bool, float> intersects(const AxisAlignedBox& box) const;
	
protected:
	Vector3f  m_origin;
	Vector3f m_direction;
};
}

#endif
