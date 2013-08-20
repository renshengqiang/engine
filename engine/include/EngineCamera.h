#ifndef _ENGINE_CAMERA_H
#define _ENGINE_CAMERA_H
#include <EngineRay.h>
#include <EngineFrustum.h>
#include <EngineMoveableObject.h>

namespace Engine
{

//cameraֻ����ˮƽ����ֱ�������������ת
class Camera : public Frustum, public MoveableObject
{
public:
	Camera(Vector3f pos=Vector3f(0,0,0),Vector3f target=Vector3f(0,0,-1), Vector3f up=Vector3f(0,1,0)):
		Frustum(pos, target, up)
	{
		//FIXME��Ҫ����boundbox�Ĵ�С
	}
	
	Matrix4f getProjViewMatrix(void);	
	
	Ray getCameraToViewportRay(float screenX, float screenY);

	//ˮƽ������ת
	inline void yaw(float angle)
	{
		m_angleHorizontal+=angle;
		m_viewChanged = true;
	}
	//��ֱ������ת
	inline void pitch(float angle)
	{
		m_angleVertical +=angle;
		m_viewChanged = true;
	}
	inline void translate(Vector3f movement)
	{
		m_positionVector += movement;
		m_viewChanged = true;
	}

protected:
	virtual void _updateViewImpl(void);
};
}

#endif