#include "EngineCamera.h"
#include "EngineMatrix.h"
#include "EngineVector.h"
#include "EngineNode.h"

namespace Engine
{
Matrix4f Camera::getProjViewMatrix(void)
{
	//m_viewChanged = true;
	if(MoveableObject::isMovded())
	{
		m_viewChanged = true;
	}
	_updateProj();
	_updateView();
	
	return m_projMatrix * m_viewMatrix;
}

void Camera::_updateViewImpl(void)
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

	Vector3f pos = m_positionVector;
	if(mp_parentNode)
	{
		pos += mp_parentNode->_getDerivedPosition();
	}
	
	m_viewMatrix.initCameraTransform(pos, m_targetVector, m_upVector);
}

Ray Camera::getCameraToViewportRay(float screenX, float screenY)
{
	Ray ret;
	Matrix4f inverseVP = getProjViewMatrix().inverse();

	float nx = (2.0f * screenX) - 1.0f;
	float ny = 1.0f - (2.0f * screenY);
	Vector3f nearPoint(nx, ny, -1.f);
	// Use midPoint rather than far point to avoid issues with infinite projection
	Vector3f midPoint (nx, ny,  0.0f);

	// Get ray origin and ray target on near plane in world space
	Vector3f rayOrigin, rayTarget;
	
	rayOrigin = inverseVP * nearPoint;
	rayTarget = inverseVP * midPoint;

	Vector3f rayDirection = rayTarget - rayOrigin;
	rayDirection.normalise();

	ret.setOrigin(rayOrigin);
	ret.setDirection(rayDirection);
	return ret;
}
}
