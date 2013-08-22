#ifndef _ENGINE_SCENE_MANAGER_H
#define _ENGINE_SCENE_MANAGER_H
#include <EngineRenderQueue.h>
#include "EngineEntity.h"
#include <vector>

namespace Engine
{
class Vector3f;
class Camera;

class  SceneManager
{
public:
	SceneManager();
	
	Camera* createCamera(const Vector3f& pos, const Vector3f target, const Vector3f up);
	
	void render();

	void _findVisibleObjects();
private:
	Camera	*mp_camera;
	Entity	*mp_entity;
	RenderQueue m_renderQueue[2];			//使用两个渲染队列供两个线程轮流使用
	int m_renderIndex[2];					//两个线程使用的index
};
}
#endif
