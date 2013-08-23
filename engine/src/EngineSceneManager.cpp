#include "EngineSceneManager.h"
#include "EngineMatrix.h"
#include "EngineQuaternion.h"
#include "EngineCamera.h"
#include "EngineSceneNode.h"
#include "EngineEntity.h"
#include <stdio.h>

namespace Engine
{
SceneManager::SceneManager():
	mp_camera(NULL)
{
	m_renderIndex[0] = m_renderIndex[1] = 0;

	mp_rootNode = new SceneNode("Root");
}

SceneManager::~SceneManager()
{
	clearScene();
	delete mp_rootNode;
}

Camera* SceneManager::createCamera(const Vector3f & pos,const Vector3f &target,const Vector3f &up)
{
	if(mp_camera)
	{
		delete mp_camera;
	}
	mp_camera = new Camera(pos, target, up);
	return mp_camera;
}

SceneNode *SceneManager::getRootNode()
{
	return mp_rootNode;
}

void SceneManager::clearScene()
{
	mp_rootNode->detachAllChildren();
	mp_rootNode->detachObject();
}

/// template method OR called strategy 
void SceneManager::_prepareRenderQueue()
{
	int index = m_renderIndex[0];

	_applySceneAnimations();
	_updateSceneGraph();
	
	m_renderQueue[index].clear();
	_findVisibleObjects(mp_camera, m_renderQueue[index]);

	m_renderIndex[0] = (m_renderIndex[0] + 1)%2; 
}

void SceneManager::_render()
{
	for(RenderQueue::Iterator it = m_renderQueue[m_renderIndex[1]].begin();
			it != m_renderQueue[m_renderIndex[1]].end();++it)
	{
		struct RenderItem &item = *it;

		item.pRenderable->render(item.transformation);
	}
	m_renderIndex[1] = (m_renderIndex[1] + 1)%2;
}

void SceneManager::_applySceneAnimations()
{
	
}
void SceneManager::_updateSceneGraph()
{
	mp_rootNode->_update(true, false);
}

void SceneManager::_findVisibleObjects(Camera *pCamera, RenderQueue &queue)
{
	mp_rootNode->_findVisibleObjects(pCamera, queue);
}

}
