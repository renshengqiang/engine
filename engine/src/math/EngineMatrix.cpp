#include "EngineMatrix.h"
#include <vector3.h>
#include <matrix3x3.h>
#include <matrix4x4.h>

namespace Engine
{
Matrix4f::Matrix4f(const aiMatrix4x4& assimpMatrix)
{
	m[0][0] = assimpMatrix.a1; m[0][1] = assimpMatrix.a2; m[0][2] = assimpMatrix.a3; m[0][3] = assimpMatrix.a4;
	m[1][0] = assimpMatrix.b1; m[1][1] = assimpMatrix.b2; m[1][2] = assimpMatrix.b3; m[1][3] = assimpMatrix.b4;
	m[2][0] = assimpMatrix.c1; m[2][1] = assimpMatrix.c2; m[2][2] = assimpMatrix.c3; m[2][3] = assimpMatrix.c4;
	m[3][0] = assimpMatrix.d1; m[3][1] = assimpMatrix.d2; m[3][2] = assimpMatrix.d3; m[3][3] = assimpMatrix.d4;
}

Matrix4f::Matrix4f(const aiMatrix3x3& assimpMatrix)
{
	m[0][0] = assimpMatrix.a1; m[0][1] = assimpMatrix.a2; m[0][2] = assimpMatrix.a3; m[0][3] = 0.0f;
	m[1][0] = assimpMatrix.b1; m[1][1] = assimpMatrix.b2; m[1][2] = assimpMatrix.b3; m[1][3] = 0.0f;
	m[2][0] = assimpMatrix.c1; m[2][1] = assimpMatrix.c2; m[2][2] = assimpMatrix.c3; m[2][3] = 0.0f;
	m[3][0] = 0.0f			 ; m[3][1] = 0.0f			;m[3][2] = 0.0f			; m[3][3] = 1.0f;
}
}
