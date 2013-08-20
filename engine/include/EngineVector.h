#ifndef _ENGINE_VECTOR_H
#define _ENGINE_VECTOR_H

#include <EngineMath.h>
#include <EngineQuaternion.h>
#include <algorithm>
#include <utility>
#include <limits>
#include <assert.h>
#include <math.h>

namespace Engine
{
//前向声明
class Matrix4f;

class Vector2i
{
public:
	int x,y;

	inline Vector2i():
		x(0), y(0)
	{
	}
};

class Vector2f
{
public:
	float x,y;

	inline Vector2f():
		x(0),y(0)
	{
	}
	inline Vector2f(float _x, float _y):
		x(_x), y(_y)
	{
	}
};

class Vector4f
{
public:
	float x,y,z,w;

	inline Vector4f():
		x(0), y(0), z(0), w(0)
	{
	}
	inline Vector4f(float _x, float _y, float _z, float _w):
		x(_x), y(_y), z(_z), w(_w)
	{
	}
	Vector4f& operator*=(const Matrix4f& r);
	Vector4f operator*(const Matrix4f& matrix);
};

class Vector3f
{
public:
	float x,y,z;

	inline Vector3f(){}
	inline Vector3f(float _x, float _y, float _z):
		x(_x), y(_y), z(_z)
	{
	}
	inline explicit Vector3f(const float fp[3]):
		x(fp[0]), y(fp[1]), z(fp[2])
	{
	}

	inline explicit Vector3f(const float scaler):
		x(scaler), y(scaler), z(scaler)
	{
	}

	//交换两个向量
	inline void swap(Vector3f& other)
	{
		std::swap(x, other.x);
		std::swap(y, other.y);
		std::swap(z, other.z);
	}

	//赋值操作
	inline const Vector3f& operator=(const Vector3f& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	inline const Vector3f& operator=(const float scaler)
	{
		x = y = z = scaler;
		return *this;
	}

	//逻辑操作
	inline bool operator == ( const Vector3f& rhs ) const
        {
            return ( x == rhs.x && y == rhs.y && z == rhs.z );
        }

        inline bool operator != ( const Vector3f& rhs ) const
        {
            return ( x != rhs.x || y != rhs.y || z != rhs.z );
        }
		
	//全部小于则返回true，否则返回false
        inline bool operator < ( const Vector3f& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }
	
	//全部大于则返回true，否则返回false
        inline bool operator > ( const Vector3f& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }
	
	//重载下标操作符，只读
	inline float operator [] ( unsigned i ) const
	{
		assert(i<3);
		return *(&x+i);
	}
	
	//重载下标操作符，可写
	inline float& operator [] ( unsigned i )
	{
		assert(i<3);
		return *(&x+i);
	}

	//重载负号运算符
	inline Vector3f operator-(void)
	{
		return Vector3f(-x, -y, -z);
	}

	//两个向量的加法
	inline const Vector3f& operator+=(const Vector3f& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	//两个向量的减法
	inline const Vector3f& operator-=(const Vector3f& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline const Vector3f& operator*=(const Vector3f& rhs)
	{
		x*=rhs.x;
		y*=rhs.y;
		z*=rhs.z;
		return *this;
	}

	inline const Vector3f& operator*=(float scale)
	{
		x*=scale;
		y*=scale;
		z*=scale;
		return *this;
	}
	
	inline const Vector3f& operator/=(const Vector3f &rhs)
	{
		x/=rhs.x;
		y/=rhs.y;
		z/=rhs.z;
		return *this;
	}

	inline const Vector3f& operator/=(float scale)
	{
		x/=scale;
		y/=scale;
		z/=scale;
		return *this;
	}

	inline Vector3f operator+(const Vector3f& rhs) const
	{
		return Vector3f(x+rhs.x, y+rhs.y, z+rhs.z);
	}

	inline Vector3f operator-(const Vector3f& rhs) const
	{
		return Vector3f(x-rhs.x, y-rhs.y, z-rhs.z);
	}
	
	inline Vector3f operator*( const float rhs ) const
	{
		return Vector3f(x * rhs, y * rhs, z * rhs);
	}
	
	inline Vector3f operator*( const Vector3f& rhs) const
	{
		return Vector3f(x * rhs.x, y * rhs.y, z * rhs.z);
	}
	
	inline Vector3f operator*(const Quaternion &q) const
	{
		// nVidia SDK implementation
		Vector3f uv, uuv;
		Vector3f qvec(q.x, q.y, q.z);
		uv = qvec.crossProduct(*this);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * q.w);
		uuv *= 2.0f;

		return *this + uv + uuv;
	}

	inline Vector3f operator/( const float fScalar ) const
	{
	   assert( fScalar != 0.0 );

	   float fInv = 1.0f / fScalar;

	   return Vector3f(
		   x * fInv,
		   y * fInv,
		   z * fInv);
	}
	
	inline Vector3f operator/( const Vector3f& rhs) const
	{
	   return Vector3f(
		   x / rhs.x,
		   y / rhs.y,
		   z / rhs.z);
	}
	
	//向量的长度
	inline float length () const
	{
		return (float)sqrt( x * x + y * y + z * z );
	}

	//向量长度的平方
	inline float squaredLength() const
	{
		return x*x + y*y + z*z;
	}

	//向量表示的两个点的距离
	inline float distance(const Vector3f& rhs) const
        {
            return (*this - rhs).length();
        }
	
        inline float squaredDistance(const Vector3f& rhs) const
        {
            return (*this - rhs).squaredLength();
        }
		
	//该向量和vec的中点
	inline Vector3f midPoint( const Vector3f& rhs) const
	{
		return Vector3f(
			(x + rhs.x)*0.5,
			(y + rhs.y)*0.5,
			(z + rhs.z)*0.5);
	}

	//向量的点乘
	inline float dotProduct(const Vector3f& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	//向量的绝对值点乘
	inline float absDotProduct(const Vector3f& vec) const
	{
	    return fabs(x * vec.x) + fabs(y * vec.y) + fabs(z * vec.z);
	}

	//规格化处理
	inline float normalise()
        {
		float fLength = (float)sqrt( x * x + y * y + z * z );

		// Will also work for zero-sized vectors, but will change nothing
		// We're not using epsilons because we don't need to.
		// Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
		if ( fLength > float(0.0f) )
		{
			float fInvLength = 1.0f / fLength;
			x *= fInvLength;
			y *= fInvLength;
			z *= fInvLength;
		}

		return fLength;
        }

	//向量叉乘
	inline Vector3f crossProduct( const Vector3f& rkVector ) const
        {
		return Vector3f(
		    y * rkVector.z - z * rkVector.y,
		    z * rkVector.x - x * rkVector.z,
		    x * rkVector.y - y * rkVector.x);
        }

	//取两个向量中的最小值成分
	inline void makeFloor( const Vector3f& cmp )
        {
		if( cmp.x < x ) x = cmp.x;
		if( cmp.y < y ) y = cmp.y;
		if( cmp.z < z ) z = cmp.z;
        }

	//取两个向量中的最大值成分
	inline void makeCeil( const Vector3f& cmp )
	{
		if( cmp.x > x ) x = cmp.x;
		if( cmp.y > y ) y = cmp.y;
		if( cmp.z > z ) z = cmp.z;
	}

	//向量围绕Axis 旋转一个角度
	void rotate(float Angle, const Vector3f& Axis);

	//特殊的向量
	static const Vector3f ZERO;
	static const Vector3f UNIT_X;
	static const Vector3f UNIT_Y;
	static const Vector3f UNIT_Z;
	static const Vector3f UNIT_SCALE;
};
}

#endif
