#ifndef _ENGINE_QUATERNION_H
#define _ENGINE_QUATERNION_H

#include <EngineMath.h>
#include <algorithm>
#include <utility>
#include <limits>
#include <assert.h>
#include <math.h>

namespace Engine
{
//forward declaration
class Matrix3f;
class Vector3f;
class Quaternion
{
/// Cutoff for sine near zero
static const float msEpsilon;

public:
	float x, y, z, w;
	
	Quaternion():
		x(0), y(0),z(0), w(1)
	{
	}
		
	Quaternion(float _x, float _y, float _z, float _w):
		x(_x), y(_y), z(_z), w(_w)
	{
	}
		
	Quaternion(const Vector3f &axis, float angle);

	bool operator==(const Quaternion &r) const
	{
		return x==r.x && y==r.y && z==r.z && w==r.w;
	}
	
	bool operator!=(const Quaternion &r) const
	{
		return !operator==(r);
	}

	Quaternion operator-() const
	{
		return Quaternion(-x,-y,-z,-w);
	}
	
	const Quaternion& operator+=(const Quaternion &r)
	{
		w += r.w;
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}
	
	Quaternion& operator-=(const Quaternion &r)
	{
		w -= r.w;
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}
	
	const Quaternion& operator*=(const Quaternion &r)
	{
		const float tw = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
		const float tx = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
		const float ty = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
		const float tz = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

		w = tw;
		x = tx;
		y = ty;
		z = tz;

		return *this;
	}
	
	Quaternion& operator*=(const Vector3f &v);
	
	Quaternion& operator*=(float t)
	{
		w *= t;
		x *= t;
		y *= t;
		z *= t;

		return *this;
	}

	Quaternion operator*(const Quaternion &rhs) const
	{
		Quaternion ret(*this);

		ret*=rhs;

		return ret;
	}
	
	Quaternion operator*(float t) const
	{
		Quaternion ret(*this);

		ret*=t;

		return ret;
	}
	
	Quaternion operator*(const Vector3f &v) const;
	
	Quaternion operator+(const Quaternion &rhs) const
	{	
		Quaternion ret(*this);

		ret+=rhs;

		return ret;
	}
	
	Quaternion operator-(const Quaternion& rhs) const
	{
		Quaternion ret(*this);

		ret-=rhs;

		return ret;
	}
	
	/// Normalises this quaternion
	void normalise()
	{
		float length = sqrtf(x * x + y * y + z * z + w * w);

		x /= length;
		y /= length;
		z /= length;
		w/= length;
	}

	// apply to non-zero quaternion
	Quaternion inverse () const
	{
		float fNorm = w*w+x*x+y*y+z*z;
		if ( fNorm > 0.0 )
		{
			float fInvNorm = 1.0f/fNorm;
			return Quaternion(-x*fInvNorm, -y*fInvNorm, -z*fInvNorm, w*fInvNorm);
		}
		else
		{
			// return an invalid result to flag the error
			return Quaternion(0,0,0,0);
		}
	}
	
	Quaternion exp () const
	{
		// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
		// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

		float fAngle ( sqrtf(x*x+y*y+z*z) );
		float fSin = sinf(fAngle);

		Quaternion kResult;
		kResult.w = cosf(fAngle);

		if (fabs(fSin) >= msEpsilon )
		{
			float fCoeff = fSin/(fAngle);
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
		}
		else
		{
			kResult.x = x;
			kResult.y = y;
			kResult.z = z;
		}

		return kResult;
	}
	
	Quaternion log () const
	{
		// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
		// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

		Quaternion kResult;
		kResult.w = 0.0;

		if ( fabs(w) < 1.0 )
		{
			float fAngle ( acosf(w) );
			float fSin = sinf(fAngle);
			if ( fabs(fSin) >= msEpsilon )
			{
				float fCoeff = fAngle/fSin;
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
				return kResult;
			}
		}

		kResult.x = x;
		kResult.y = y;
		kResult.z = z;

		return kResult;
	}
	
	//共轭四元数
	Quaternion conjugate() const
	{
		Quaternion ret(-x, -y, -z, w);
		return ret;
	}
	
	/// translate the quaternion to transform matrix
	Matrix3f toRotationMatrix (void) const;
	
	// functions of a quaternion
	/// Returns the dot product of the quaternion
	float dotProduct (const Quaternion &rkQ) const
	{
		return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
	}
	
	/** Performs Normalised linear interpolation between two quaternions, and returns the result.
		nlerp ( 0.0f, A, B ) = A
		nlerp ( 1.0f, A, B ) = B
		@remarks
		Nlerp is faster than Slerp.
		Nlerp has the proprieties of being commutative (@see Slerp;
		commutativity is desired in certain places, like IK animation), and
		being torque-minimal (unless shortestPath=false). However, it's performing
		the interpolation at non-constant velocity; sometimes this is desired,
		sometimes it is not. Having a non-constant velocity can produce a more
		natural rotation feeling without the need of tweaking the weights; however
		if your scene relies on the timing of the rotation or assumes it will point
		at a specific angle at a specific weight value, Slerp is a better choice.
	*/
	static Quaternion nlerp(float fT, const Quaternion &rkP,const Quaternion &rkQ, bool shortestPath = false);
	
	/** Performs Spherical linear interpolation between two quaternions, and returns the result.
		Slerp ( 0.0f, A, B ) = A
		Slerp ( 1.0f, A, B ) = B
		@return Interpolated quaternion
		@remarks
		Slerp has the proprieties of performing the interpolation at constant
		velocity, and being torque-minimal (unless shortestPath=false).
		However, it's NOT commutative, which means
		Slerp ( 0.75f, A, B ) != Slerp ( 0.25f, B, A );
		therefore be careful if your code relies in the order of the operands.
		This is specially important in IK animation.
	*/
	static Quaternion slerp(float fT, const Quaternion &rkP,const Quaternion &rkQ, bool shortestPath = false);

	// spherical quadratic interpolation
	static Quaternion squad (float fT,	const Quaternion &rkP, const Quaternion &rkA,
							const Quaternion &rkB, const Quaternion &rkQ, bool shortestPath = false);
	};
}

#endif
