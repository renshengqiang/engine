#ifndef _ENGINE_PLANE_H__
#define _ENGINE_PLANE_H__

#include <EngineVector.h>
#include <EngineMatrix.h>
#include <EngineAxisAlignedBox.h>
#include <vector>

namespace Engine
{
/**  ����ඨ����3D�ռ��е�һ��ƽ��
	���ƽ��������ķ��̶���:
	Ax + By + Cz + D = 0
	����(A,B,C)�Ǹ�ƽ��ķ���
	D��ԭ�㰴�շ��߷������ƽ��ľ���
 */
class Plane
{
friend class Frustum;
private:
	Vector3f 	m_normal;
	float		m_distance;
	
public:
	inline Plane():
		m_normal(Vector3f(0, 1, 0)),
		m_distance(0)
	{
	}
	
	inline Plane(const Plane& rhs)
	{
		m_normal = rhs.m_normal;
		m_distance = rhs.m_distance;
	}
	
	inline Plane(const Vector3f& normal, float distance)
	{
		m_normal = normal;
		m_distance = distance;
	}
	
	inline Plane(float a, float b, float c, float d)
	{
		m_normal = Vector3f(a,b,c);
		m_distance = d;
	}
	
	inline Plane(const Vector3f& normal, const Vector3f& point)
	{
		redefine(normal, point);
	}
	
	inline Plane(const Vector3f& point0, const Vector3f& point1, const Vector3f& point2)
	{
		redefine(point0, point1, point2);
	}

	/** The "positive side" of the plane is the half space to which the
	plane normal points. The "negative side" is the other half
	space. The flag "no side" indicates the plane itself.
	*/
	enum Side
	{
		NO_SIDE,
		POSITIVE_SIDE,
		NEGATIVE_SIDE,
		BOTH_SIDE
	};

	/**	�ж�һ������ƽ�����һ��
		ֻ�����ֿ���:positive, negative, no side
	*/
	inline Side getSide(const Vector3f& point) const
	{
		float distance = getDistance(point);

		if (distance < 0.0 )
			return Plane::NEGATIVE_SIDE;

		if (distance > 0.0 )
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}
	
	/**	�ж�һ����Χ����ƽ�����һ��
	*/
	inline Side getSide(const AxisAlignedBox& box) const
	{
		if (box.isNull()) 
			return NO_SIDE;
		if (box.isInfinite())
			return BOTH_SIDE;

		return getSide(box.getCenter(), box.getHalfSize());
	}

	inline Side getSide(const Vector3f& centre, const Vector3f& halfSize) const
	{
		// Calculate the distance between box centre and the plane
		float dist = getDistance(centre);

		// Calculate the maximise allows absolute distance for
		// the distance between box centre and plane
		float maxAbsDist = m_normal.absDotProduct(halfSize);

		if (dist < -maxAbsDist)
			return Plane::NEGATIVE_SIDE;

		if (dist > +maxAbsDist)
			return Plane::POSITIVE_SIDE;

		return Plane::BOTH_SIDE;
	}

	/**	����һ���㵽��ƽ��ľ���*/
	inline float getDistance(const Vector3f& point) const
	{
		return m_normal.dotProduct(point) + m_distance;
	}

	/** Redefine this plane based on 3 points. */
	inline void redefine(const Vector3f& point0, const Vector3f& point1, const Vector3f& point2)
	{
		Vector3f kEdge1 = point1 - point0;
		Vector3f kEdge2 = point2 - point0;
		m_normal = kEdge1.crossProduct(kEdge2);
		m_normal.normalise();
		m_distance= -m_normal.dotProduct(point0);
	}
	
	/** Redefine this plane based on a normal and a point. */
 	inline void redefine(const Vector3f& normal, const Vector3f& point)
 	{
 		m_normal = normal;
		m_distance= -normal.dotProduct(point);
 	}
	
	/** Project a vector onto the plane. 
	@remarks This gives you the element of the input vector that is perpendicular 
		to the normal of the plane. You can get the element which is parallel
		to the normal of the plane by subtracting the result of this method
		from the original vector, since parallel + perpendicular = original.
	@param v The input vector
	*/
	inline Vector3f projectVector(const Vector3f& v) const
	{
		// We know plane normal is unit length, so use simple method
		Matrix3f xform;
		xform[0][0] = 1.0f - m_normal.x * m_normal.x;
		xform[0][1] = -m_normal.x * m_normal.y;
		xform[0][2] = -m_normal.x * m_normal.z;
		xform[1][0] = -m_normal.y * m_normal.x;
		xform[1][1] = 1.0f - m_normal.y * m_normal.y;
		xform[1][2] = -m_normal.y * m_normal.z;
		xform[2][0] = -m_normal.z * m_normal.x;
		xform[2][1] = -m_normal.z * m_normal.y;
		xform[2][2] = 1.0f - m_normal.z * m_normal.z;
		return xform * v;
	}

	/** Normalises the plane.
	    @remarks
	        This method normalises the plane's normal and the length scale of d
	        is as well.
	    @note
	        This function will not crash for zero-sized vectors, but there
	        will be no changes made to their components.
	    @return The previous length of the plane's normal.
	*/
	inline float normalise(void)
	{
		float fLength = m_normal.length();

		// Will also work for zero-sized vectors, but will change nothing
		// We're not using epsilons because we don't need to.
		// Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
		if ( fLength > float(0.0f) )
		{
			float fInvLength = 1.0f / fLength;
			m_normal *= fInvLength;
			m_distance*= fInvLength;
		}

		return fLength;
	}

	/// Comparison operator
	inline bool operator==(const Plane& rhs) const
	{
	    return (rhs.m_distance == m_distance && rhs.m_normal == m_normal);
	}
	inline bool operator!=(const Plane& rhs) const
	{
	    return (rhs.m_distance != m_distance || rhs.m_normal != m_normal);
	}
};
typedef std::vector<Plane> PlaneList;
}
#endif
