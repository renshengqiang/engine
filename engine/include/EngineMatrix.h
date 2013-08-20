#ifndef _ENGINE_MATRIX_H
#define _ENGINE_MATRIX_H

#include <EngineMath.h>
#include <EngineVector.h>
#include <EngineQuaternion.h>
#include <algorithm>
#include <utility>
#include <limits>
#include <assert.h>
#include <math.h>

template <typename TYPE>
class aiMatrix3x3t;
template <typename TYPE>
class aiMatrix4x4t;
typedef class aiMatrix3x3t<float> aiMatrix3x3;
typedef class aiMatrix4x4t<float> aiMatrix4x4;
namespace Engine
{
class Matrix3f
{
public:
	float m[3][3];
	
	inline float* operator[](int  iRow)
        {
        	return (float*)m[iRow];
        }
	/// Matrix * vector [3x3 * 3x1 = 3x1]
        inline Vector3f operator* (const Vector3f& rkVector) const
	{
		Vector3f kProd;
		for (int iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				m[iRow][0]*rkVector[0] +
				m[iRow][1]*rkVector[1] +
				m[iRow][2]*rkVector[2];
		}
		return kProd;
	}
	inline void fromAxes(const Vector3f& xAxis, const Vector3f& yAxis, const Vector3f& zAxis)
	{
		m[0][0] = xAxis.x; m[1][0] = xAxis.y; m[2][0] = xAxis.z;
		m[0][1] = yAxis.x; m[1][1] = yAxis.y; m[2][1] = yAxis.z;
		m[0][2] = zAxis.x; m[1][2] = zAxis.y; m[2][2] = zAxis.z;
	}
};

class Matrix4f
{
public:
	float m[4][4];

	inline Matrix4f()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
	
	Matrix4f(const aiMatrix4x4& assimpMatrix);
	Matrix4f(const aiMatrix3x3& assimpMatrix);
	
	inline Matrix4f(float a00, float a01, float a02, float a03,
	         float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23,
	         float a30, float a31, float a32, float a33)
	{
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
		m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33; 
	}
	
	inline const Matrix4f& operator=( const Matrix3f& mat3 )	
	{
		m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
		m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
		m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
		return *this;
	}

	inline float* operator [] (size_t  iRow)
        {
            assert( iRow < 4 );
            return m[iRow];
        }
	
        inline const float* operator [] (size_t iRow ) const
        {
            assert( iRow < 4 );
            return m[iRow];
        }
	
	inline Matrix4f& operator+=(const Matrix4f &rhs)
	{
		for(int i=0;i<4;++i)
			for(int j=0;j<4;++j)
				m[i][j] += rhs.m[i][j];
		return *this;
	}
	
	inline Matrix4f operator*(const Matrix4f& rhs) const
	{
		Matrix4f ret;
		
		for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				ret.m[i][j] = m[i][0] * rhs.m[0][j] +
							  m[i][1] * rhs.m[1][j] +
							  m[i][2] * rhs.m[2][j] +
							  m[i][3] * rhs.m[3][j];
			}
		}
		
		return ret;
	}
	
	inline const Matrix4f &operator*=(float weight)
	{
		for(int i=0;i<4;++i)
			for(int j=0;j<4;++j)
				m[i][j] = m[i][j] * weight;
		return *this;
	}

	inline Matrix4f operator*(float weight) const
	{
		Matrix4f ret(*this);

		ret*=weight;;

		return ret;
	}

	/*
		Vector transformation using '*'
		@remarks
		transforms the given 3-D vector by the matrix, projection the result back into w=1
		@note
		This meas that the initial w=1.0, and then all the three elements of the resulting 3-D vector are diveded by the resulting w
	*/
	inline Vector3f operator*(const Vector3f &V) const
	{
		Vector3f ret;
		float fInvW = 1.0f /(m[3][0] * V.x + m[3][1] * V.y + m[3][2] * V.z + m[3][3]);
	
		ret.x = (m[0][0]*V.x + m[0][1]*V.y + m[0][2]*V.z + m[0][3])*fInvW;
		ret.y = (m[1][0]*V.x + m[1][1]*V.y + m[1][2]*V.z + m[1][3])*fInvW;
		ret.z = (m[2][0]*V.x + m[2][1]*V.y + m[2][2]*V.z + m[2][3])*fInvW;
		return ret;
	}
	
	// 转置矩阵,不破坏当前矩阵
	inline Matrix4f transposition() const
	{
		Matrix4f n;
	
		for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				n.m[i][j] = m[j][i];
			}
		}
		return n;
	}

	//计算对应行列式的值
	inline float determinant() const
	{
		return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3] 
			+ m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3] 
			- m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2] 
			+ m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1] 
			+ m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
			- m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
	}
	
	// 逆矩阵，按照公式A=(1/|a|)*(A*)，其中(A*)是矩阵A的伴随矩阵
	inline const Matrix4f& inverse()
	{
		// 先计算行列式的值
		float det = determinant();
		if(det == 0.0f) 
		{
			// Matrix not invertible. Setting all elements to nan is not really
			// correct in a mathematical sense but it is easy to debug for the
			// programmer.
			const float nan = std::numeric_limits<float>::quiet_NaN();
			*this = Matrix4f(
				nan,nan,nan,nan,
				nan,nan,nan,nan,
				nan,nan,nan,nan,
				nan,nan,nan,nan);
	       		 assert(0);
			return *this;
		}

		float invdet = 1.0f / det;

		Matrix4f res;
		res.m[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
		res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
		res.m[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
		res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
		res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
		res.m[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
		res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
		res.m[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
		res.m[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
		res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
		res.m[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
		res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
		res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
		res.m[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
		res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
		res.m[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0])); 
		*this = res;

		return *this;
	}

	//是否是仿射变换矩阵，即最后一行是否为0 0 0 1
	inline bool isAffine(void) const
        {
            return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
        }
	
	/** 3-D Vector transformation specially for an affine matrix.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        inline Vector3f transformAffine(const Vector3f& v) const
        {
            assert(isAffine());

            return Vector3f(
                    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3], 
                    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
        }

        /** 4-D Vector transformation specially for an affine matrix.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        inline Vector4f transformAffine(const Vector4f& v) const
        {
            assert(isAffine());

            return Vector4f(
                m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                v.w);
        }
		
	//初始化为单位矩阵
	inline void initIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
	
	//初始化为零矩阵
	inline void InitZero()
	{
		m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;
	}
	
	inline void initScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
	{
		m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
   	 	m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
	}
	
	inline void initRotateTransform(float RotateX, float RotateY, float RotateZ)
	{
		Matrix4f rx, ry, rz;

		const float x = ToRadian(RotateX);
		const float y = ToRadian(RotateY);
		const float z = ToRadian(RotateZ);

		rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
		rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
		rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
		rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

		ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = sinf(y); ry.m[0][3] = 0.0f;
		ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
		ry.m[2][0] = -sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
		ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

		rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
		rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
		rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
		rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

		*this = rz * ry * rx;
	}
	
	inline void initTranslationTransform(float x, float y, float z)
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
	
	inline void initTranslationTransform(Vector3f trans)
	{
		initTranslationTransform(trans.x, trans.y, trans.z);
	}
	
	inline void initCameraTransform(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
	{
		Vector3f N = target;
		N.normalise();
		Vector3f U = up;
		U.normalise();
		U = N.crossProduct(U);
		U.normalise();
		Vector3f V = U.crossProduct(N);
		V.normalise();

		m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
		m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
		m[2][0] = -N.x;   m[2][1] = -N.y;   m[2][2] = -N.z;   m[2][3] = 0.0f;
		m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
		
		//multiply pos
		m[0][3] = -(pos.x*U.x + pos.y*U.y + pos.z*U.z);
		m[1][3] = -(pos.x*V.x + pos.y*V.y + pos.z*V.z);
		m[2][3] = (pos.x*N.x + pos.y*N.y + pos.z*N.z);
	}
	
	void initPersProjTransform(float FOV, float ar, float zNear, float zFar)
	{
		const float zRange	   = zNear - zFar;
		const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));
		
		m[0][0] = 1.0f/(tanHalfFOV * ar);   m[0][1] = 0.0f;			 m[0][2] = 0.0f;		  			m[0][3] = 0.0;
		m[1][0] = 0.0f; 				  m[1][1] = 1.0f/tanHalfFOV;    m[1][2] = 0.0f;		  			m[1][3] = 0.0;
		m[2][0] = 0.0f; 				  m[2][1] = 0.0f;			 m[2][2] = (zNear + zFar)/zRange ; 	m[2][3] = 2.0f * zFar*zNear/zRange;
		m[3][0] = 0.0f; 				  m[3][1] = 0.0f;			 m[3][2] = -1.0f;		  			m[3][3] = 0.0;
	}
	
	void makeTransformMatrix(const Vector3f& position, const Vector3f& scale, const Quaternion& orientation)
	{
		// Ordering:实际效果的顺序，非代码顺序
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		Matrix3f rot3x3 = orientation.toRotationMatrix();

		// Set up final matrix with scale, rotation and translation
		m[0][0] = scale.x * rot3x3.m[0][0]; m[0][1] = scale.y * rot3x3.m[0][1]; m[0][2] = scale.z * rot3x3.m[0][2]; m[0][3] = position.x;
		m[1][0] = scale.x * rot3x3.m[1][0]; m[1][1] = scale.y * rot3x3.m[1][1]; m[1][2] = scale.z * rot3x3.m[1][2]; m[1][3] = position.y;
		m[2][0] = scale.x * rot3x3.m[2][0]; m[2][1] = scale.y * rot3x3.m[2][1]; m[2][2] = scale.z * rot3x3.m[2][2]; m[2][3] = position.z;

		// No projection term
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
};
}

#endif
