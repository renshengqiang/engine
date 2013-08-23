#ifndef _ENGINE_SCENE_MANAGER_H
#define _ENGINE_SCENE_MANAGER_H
#include <EngineRenderQueue.h>
#include <vector>

namespace Engine
{
class Vector3f;
class Camera;
class Entity;
class SceneNode;
class  SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	
	Camera* createCamera(const Vector3f& pos, const Vector3f &target, const Vector3f &up);
	SceneNode *getRootNode();

	void clearScene();

	void _prepareRenderQueue();
	void _render();
	
protected:
	virtual void _updateSceneGraph();
	virtual void _applySceneAnimations();
	virtual void _findVisibleObjects(Camera *pCamera, RenderQueue &queue);
private:
	Camera	*mp_camera;
	Entity	*mp_entity;
	RenderQueue m_renderQueue[2];			//ʹ��������Ⱦ���й������߳�����ʹ��
	int m_renderIndex[2];					//�����߳�ʹ�õ�index
	SceneNode *mp_rootNode;
};
}
#endif
