#include "EngineSceneManager.h"
#include "EngineMatrix.h"
#include "EngineQuaternion.h"
#include "EngineCamera.h"
#include <stdio.h>

namespace Engine
{
SceneManager::SceneManager():
	mp_camera(NULL)
{
	m_renderIndex[0] = m_renderIndex[1] = 0;
	/// test use
	mp_entity = new Entity("./media/models/phoenix_ugv.md2");
}

Camera* SceneManager::createCamera(const Vector3f & pos,const Vector3f target,const Vector3f up)
{
	if(mp_camera)
	{
		delete mp_camera;
	}
	mp_camera = new Camera(pos, target, up);
	return mp_camera;
}

void SceneManager::_findVisibleObjects()
{
	m_renderQueue[m_renderIndex[0]].clear();

	/// test use
	Matrix4f projViewMatrix = mp_camera->getProjViewMatrix();
	Matrix4f transMatrix;

	transMatrix.makeTransformMatrix(Vector3f(0, 0, -200), 
								Vector3f(1, 1, 1), 
								Quaternion(Vector3f(0, 1, 0), 45));

	/// FIXME: add moveable object map, and iterate it
	mp_entity->addToRenderQueue(projViewMatrix * transMatrix, m_renderQueue[m_renderIndex[0]]);
	m_renderIndex[0] = (m_renderIndex[0] + 1)%2;
}

void SceneManager::render()
{
	for(RenderQueue::Iterator it = m_renderQueue[m_renderIndex[1]].begin();
			it != m_renderQueue[m_renderIndex[1]].end();
			++it)
	{
		struct RenderItem &item = *it;

		item.pRenderable->render(item.transformation);
	}
};
}
