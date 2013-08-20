#include "EngineFrustum.h"

namespace Engine
{
Frustum::Frustum(Vector3f pos,Vector3f target,Vector3f up):
	m_positionVector(pos),
	m_targetVector(target),
	m_upVector(up),
	m_FOVy(45.0f),
	m_nearDist(1),	
	m_farDist(10000),
	m_aspectRadio(1.333333333f),
	m_viewChanged(true),
	m_projChanged(true),
	m_frustumPlanesChanged(true)
{	
	m_targetVector.normalise();
	m_upVector.normalise();
	
	//Initialize two angles
	Vector3f targetVector(m_targetVector.x, 0.0f, m_targetVector.z);//horizontal target
	targetVector.normalise();
	if(targetVector.z >= 0.0f)
	{
		if(targetVector.x >= 0.0f)
		{
			m_angleHorizontal = 360.0f - ToDegree(asin(targetVector.z));
		}
		else
		{
			m_angleHorizontal = 180.0f + ToDegree(asin(targetVector.z));
		}
	}
	else
	{
		if(targetVector.x >= 0.0f)
		{
			m_angleHorizontal = ToDegree(asin(-targetVector.z));
		}else
		{
			m_angleHorizontal = 90.0f + ToDegree(asin(-targetVector.z));
		}
	}
	m_angleVertical = -ToDegree(asin(m_targetVector.y));
}

bool Frustum::isVisible(const AxisAlignedBox& bound, FrustumPlane *culledBy)
{
	// Null boxes always invisible
	if (bound.isNull()) return false;

	// Infinite boxes always visible
	if (bound.isInfinite()) return true;

	// Make any pending updates to the calculated frustum planes
	_updateFrustumPlanes();

	// Get centre of the box
	Vector3f centre = bound.getCenter();
	// Get the half-size of the box
	Vector3f halfSize = bound.getHalfSize();

	// For each plane, see if all points are on the negative side
	// If so, object is not visible
	for (int plane = 0; plane < 6; ++plane)
	{
		// Skip far plane if infinite view frustum
		if (plane == FRUSTUM_PLANE_FAR && m_farDist == 0)
			continue;

		Plane::Side side = m_frustumPlanes[plane].getSide(centre, halfSize);
		if (side == Plane::NEGATIVE_SIDE)
		{
			// ALL corners on negative side therefore out of view
			if (culledBy)
				*culledBy = (FrustumPlane)plane;
			return false;
		}
	}
	return true;
}

void Frustum::_updateViewImpl(void)
{
	const Vector3f Vaxis(0.0f, 1.0f, 0.0f);
	Vector3f View(1.0f, 0.0f, 0.0f);
	View.rotate(m_angleHorizontal, Vaxis);
	View.normalise();

	Vector3f Haxis = View.crossProduct(Vaxis);
	Haxis.normalise();
	View.rotate(m_angleVertical, Haxis);

	m_targetVector= View;
	m_targetVector.normalise();

	m_upVector = Haxis.crossProduct(m_targetVector);
	m_upVector.normalise();
	m_viewMatrix.initCameraTransform(m_positionVector,m_targetVector,m_upVector);
}

void Frustum::_updateProjImpl(void)
{
	m_projMatrix.initPersProjTransform(m_FOVy, m_aspectRadio,m_nearDist, m_farDist);
}

void Frustum::_updateFrustumPlanesImpl(void)
{
	// -------------------------
	// Update the frustum planes
	// -------------------------
	Matrix4f combo = m_projMatrix * m_viewMatrix;

	m_frustumPlanes[FRUSTUM_PLANE_LEFT].m_normal.x = combo[3][0] + combo[0][0];
	m_frustumPlanes[FRUSTUM_PLANE_LEFT].m_normal.y = combo[3][1] + combo[0][1];
	m_frustumPlanes[FRUSTUM_PLANE_LEFT].m_normal.z = combo[3][2] + combo[0][2];
	m_frustumPlanes[FRUSTUM_PLANE_LEFT].m_distance = combo[3][3] + combo[0][3];

	m_frustumPlanes[FRUSTUM_PLANE_RIGHT].m_normal.x = combo[3][0] - combo[0][0];
	m_frustumPlanes[FRUSTUM_PLANE_RIGHT].m_normal.y = combo[3][1] - combo[0][1];
	m_frustumPlanes[FRUSTUM_PLANE_RIGHT].m_normal.z = combo[3][2] - combo[0][2];
	m_frustumPlanes[FRUSTUM_PLANE_RIGHT].m_distance = combo[3][3] - combo[0][3];

	m_frustumPlanes[FRUSTUM_PLANE_TOP].m_normal.x = combo[3][0] - combo[1][0];
	m_frustumPlanes[FRUSTUM_PLANE_TOP].m_normal.y = combo[3][1] - combo[1][1];
	m_frustumPlanes[FRUSTUM_PLANE_TOP].m_normal.z = combo[3][2] - combo[1][2];
	m_frustumPlanes[FRUSTUM_PLANE_TOP].m_distance = combo[3][3] - combo[1][3];

	m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].m_normal.x = combo[3][0] + combo[1][0];
	m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].m_normal.y = combo[3][1] + combo[1][1];
	m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].m_normal.z = combo[3][2] + combo[1][2];
	m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].m_distance = combo[3][3] + combo[1][3];

	m_frustumPlanes[FRUSTUM_PLANE_NEAR].m_normal.x = combo[3][0] + combo[2][0];
	m_frustumPlanes[FRUSTUM_PLANE_NEAR].m_normal.y = combo[3][1] + combo[2][1];
	m_frustumPlanes[FRUSTUM_PLANE_NEAR].m_normal.z = combo[3][2] + combo[2][2];
	m_frustumPlanes[FRUSTUM_PLANE_NEAR].m_distance = combo[3][3] + combo[2][3];

	m_frustumPlanes[FRUSTUM_PLANE_FAR].m_normal.x = combo[3][0] - combo[2][0];
	m_frustumPlanes[FRUSTUM_PLANE_FAR].m_normal.y = combo[3][1] - combo[2][1];
	m_frustumPlanes[FRUSTUM_PLANE_FAR].m_normal.z = combo[3][2] - combo[2][2];
	m_frustumPlanes[FRUSTUM_PLANE_FAR].m_distance = combo[3][3] - combo[2][3];

	// Renormalise any normals which were not unit length
	for(int i=0; i<6; i++ )
	{
		float length = m_frustumPlanes[i].m_normal.normalise();
		m_frustumPlanes[i].m_distance /= length;
	}
}
}
