#include "EngineQuaternion.h"
#include "EngineVector.h"
#include "EngineMatrix.h"

namespace Engine{

const float Quaternion::msEpsilon = 1e-03;

Quaternion::Quaternion(const Vector3f& axis, float angle)
{
	float fHalfAngle ( 0.5*ToRadian(angle));
	float fSin = sinf(fHalfAngle);
	w = cosf(fHalfAngle);
	x = fSin*axis.x;
	y = fSin*axis.y;
	z = fSin*axis.z;
}

Quaternion& Quaternion::operator*=(const Vector3f &v)
{
	const float tw = - (x * v.x) - (y * v.y) - (z * v.z);
	const float tx =   (w * v.x) + (y * v.z) - (z * v.y);
	const float ty =   (w * v.y) + (z * v.x) - (x * v.z);
	const float tz =   (w * v.z) + (x * v.y) - (y * v.x);

	w = tw;
	x = tx;
	y = ty;
	z = tz;

	return *this;
}

Quaternion Quaternion::operator*(const Vector3f& v) const
{
	Quaternion ret(*this);

	ret*=v;

	return ret;
}

Matrix3f Quaternion::toRotationMatrix (void) const
{
	Matrix3f ret;
	
	float fTx  = x+x;
	float fTy  = y+y;
	float fTz  = z+z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;
	float fTzz = fTz*z;

	ret.m[0][0] = 1.0f-(fTyy+fTzz);
	ret.m[0][1] = fTxy-fTwz;
	ret.m[0][2] = fTxz+fTwy;
	ret.m[1][0] = fTxy+fTwz;
	ret.m[1][1] = 1.0f-(fTxx+fTzz);
	ret.m[1][2] = fTyz-fTwx;
	ret.m[2][0] = fTxz-fTwy;
	ret.m[2][1] = fTyz+fTwx;
	ret.m[2][2] = 1.0f-(fTxx+fTyy);

	return ret;
}

/*
	为什么下面是-rkQ?
	因为两者角度大于180度时，需要把第二个角度减去360度，表示从反方向变换到rkQ
	令本来rkQ的角度是r, 变过以后就是r-360
	而四元数采取的是cos(r/2), sin(r/2)*xxx
	于是r/2 变成r/2 -180
	而cos(r/2-180) =-cos(r/2), sin(r/2-180) =-sin(r/2)
	所以得到的是-rkQ
*/
Quaternion Quaternion::nlerp(float fT, const Quaternion& rkP,
	const Quaternion& rkQ, bool shortestPath)
{
	Quaternion result;
	
	float fCos = rkP.dotProduct(rkQ);
	if (fCos < 0.0f && shortestPath)
	{
		result = rkP + ((-rkQ) - rkP)*fT;
	}
	else
	{
		result = rkP +(rkQ - rkP)*fT;
	}
	result.normalise();
	return result;
}

/*
	球形插值: 以恒定的速率描述两四元数之间的曲线
	即得到的效果是角度的线性插值θ=(1-t)*θ1+t*θ2
	结论是: q(t) = q1*sin(1-t)(θ)/sin(θ) +q2*sin(tθ)/sin(θ)
	因为涉及到tan的计算，所以需要对cos的值进行一个范围的判断，防止越界
*/
Quaternion Quaternion::slerp (float fT, const Quaternion& rkP,
	  const Quaternion& rkQ, bool shortestPath)
  {
	  float fCos = rkP.dotProduct(rkQ);
	  Quaternion rkT;

	  // Do we need to invert rotation?
	  if (fCos < 0.0f && shortestPath)
	  {
		  fCos = -fCos;
		  rkT = -rkQ;
	  }
	  else
	  {
		  rkT = rkQ;
	  }

	  if (fabs(fCos) < 1 - msEpsilon)
	  {
		  // Standard case (slerp)
		  float fSin = sqrtf(1 - fCos*fCos);
		  float fAngle = atan2f(fSin, fCos);
		  float fInvSin = 1.0f / fSin;
		  float fCoeff0 = sinf((1.0f - fT) * fAngle) * fInvSin;
		  float fCoeff1 = sinf(fT * fAngle) * fInvSin;
		  return  rkP *fCoeff0 + rkT*fCoeff1;
	  }
	  else
	  {
		  // There are two situations:
		  // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
		  //	interpolation safely.
		  // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
		  //	are an infinite number of possibilities interpolation. but we haven't
		  //	have method to fix this case, so just use linear interpolation here.
		  Quaternion t =  rkP  * (1.0f - fT)+ rkT * fT ;
		  // taking the complement requires renormalisation
		  t.normalise();
		  return t;
	  }
  }

Quaternion Quaternion::squad (float fT,
	const Quaternion& rkP, const Quaternion& rkA,
	const Quaternion& rkB, const Quaternion& rkQ, bool shortestPath)
{
	float fSlerpT = 2.0f*fT*(1.0f-fT);
	Quaternion kSlerpP = slerp(fT, rkP, rkQ, shortestPath);
	Quaternion kSlerpQ = slerp(fT, rkA, rkB);
	return slerp(fSlerpT, kSlerpP ,kSlerpQ);
}

}
