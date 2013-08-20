#ifndef _ENGINE_FRUSTRUM_H
#define _ENGINE_FRUSTRUM_H

#include <EngineVector.h>
#include <EngineMatrix.h>
#include <EnginePlane.h>

namespace Engine
{
/** Worldspace clipping planes.
*/
enum FrustumPlane
{
	FRUSTUM_PLANE_NEAR	 = 0,
	FRUSTUM_PLANE_FAR	 = 1,
	FRUSTUM_PLANE_LEFT	 = 2,
	FRUSTUM_PLANE_RIGHT  = 3,
	FRUSTUM_PLANE_TOP	 = 4,
	FRUSTUM_PLANE_BOTTOM = 5
};

class Frustum
{
public:
	Frustum(Vector3f pos=Vector3f(0,0,0), Vector3f target=Vector3f(0,0,-1), Vector3f up=Vector3f(0,1,0));
	
	inline void setFov(float angle)
	{
		m_FOVy = angle;
		m_projChanged = true;
	}
	
	inline void setClipDistance(float near, float far)
	{
		m_nearDist = near;
		m_farDist = far;
		m_projChanged = true;
	}
	
	inline void setAspetcRadio(float aspectRadio)
	{
		m_aspectRadio= aspectRadio;
		m_projChanged = true;
	}
	
	bool isVisible(const AxisAlignedBox& bound, FrustumPlane *culledBy);
	
protected:
	inline void _updateView(void)
	{
		if(m_viewChanged)
		{
			_updateViewImpl();
			m_viewChanged = false;
			m_frustumPlanesChanged = true;
		}
	}
	
	inline void _updateProj(void)
	{
		if(m_projChanged)
		{
			_updateProjImpl();
			m_projChanged = false;
			m_frustumPlanesChanged = true;
		}
	}
	
	inline void _updateFrustumPlanes(void)
	{
		_updateView();
		_updateProj();
		if(m_frustumPlanesChanged)
		{
			_updateFrustumPlanesImpl();
			m_frustumPlanesChanged = false;
		}
	}
	virtual void _updateViewImpl(void);
	virtual void _updateProjImpl(void);
	virtual void _updateFrustumPlanesImpl(void);
	
protected:
	//view matrix 相关变量
	Vector3f m_positionVector;
	Vector3f m_targetVector;
	Vector3f m_upVector;
	mutable Matrix4f m_viewMatrix;

	//projection matrix相关变量
	float m_FOVy;/*以角度计的Y方向的视角角度*/
	float m_nearDist, m_farDist;
	float m_aspectRadio;/*aspect radio = screen_width/screen_height*/
	mutable Matrix4f m_projMatrix;			

	mutable Plane m_frustumPlanes[6];
	mutable bool m_viewChanged, m_projChanged, m_frustumPlanesChanged;
	
	/*水平方向上的旋转角度，X正轴方向是0度，以度数计*/
	mutable float m_angleHorizontal;
	/*竖直方向上的旋转角度，Z负轴方向是0度，以度数计*/
	mutable float m_angleVertical;
};

}

#endif
