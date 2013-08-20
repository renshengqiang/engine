#include "EngineMatrix.h"

namespace Engine
{
	const Vector3f Vector3f::ZERO( 0, 0, 0 );
	const Vector3f Vector3f::UNIT_X( 1, 0, 0 );
	const Vector3f Vector3f::UNIT_Y( 0, 1, 0 );
	const Vector3f Vector3f::UNIT_Z( 0, 0, 1 );
	const Vector3f Vector3f::UNIT_SCALE(1, 1, 1);

	Vector4f& Vector4f::operator*=(const Matrix4f& r)
	{
		x = x*r[0][0] + y*r[1][0] + z*r[2][0] + w*r[3][0];
		y = x*r[0][1] + y*r[1][1] + z*r[2][1] + w*r[3][1];
		z = x*r[0][2] + y*r[1][2] + z*r[2][2] + w*r[3][2];
		w= x*r[0][3] + y*r[1][3] + z*r[2][3] + w*r[3][3];
		return *this;
	}
	Vector4f Vector4f::operator*(const Matrix4f& matrix)
	{
		Vector4f ret(*this);
		ret*=matrix;
		return ret;
	}

	void Vector3f::rotate(float Angle, const Vector3f& Axis)
	{	
		const float SinHalfAngle = sinf(ToRadian(Angle/2));
		const float CosHalfAngle = cosf(ToRadian(Angle/2));

		const float Rx = Axis.x * SinHalfAngle;
		const float Ry = Axis.y * SinHalfAngle;
		const float Rz = Axis.z * SinHalfAngle;
		const float Rw = CosHalfAngle;
		Quaternion RotationQ(Rx, Ry, Rz, Rw);

		Quaternion ConjugateQ = RotationQ.conjugate();
		//ConjugateQ.Normalize();
		Quaternion W = RotationQ * (*this) * ConjugateQ;

		x = W.x;
		y = W.y;
		z = W.z;
	}
}
