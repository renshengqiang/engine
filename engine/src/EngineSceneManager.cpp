#include "EngineSceneManager.h"
#include "EngineMesh.h"
#include "EngineSimpleMeshEffect.h"
#include "EngineMatrix.h"
#include "EngineQuaternion.h"
#include "EngineCamera.h"
#include "EngineEffectManager.h"
#include "EngineMeshManager.h"
#include <stdio.h>

namespace Engine
{
SceneManager::SceneManager():
	mp_camera(NULL)
{
	MeshManager *pMeshManager = MeshManager::getSingletonPtr();
	m_meshPtr = pMeshManager->createMesh("./media/models/phoenix_ugv.md2");
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

void SceneManager::render()
{
	EffectManager *pEffectManager = EffectManager::getSingletonPtr();
	mp_effect = static_cast<SimpleMeshEffect*>(pEffectManager->getEffec("SubEntity"));
	mp_effect->enable();

	Matrix4f projViewMatrix = mp_camera->getProjViewMatrix();
	Matrix4f transMatrix;

	transMatrix.makeTransformMatrix(Vector3f(0, 0, -200), 
								Vector3f(1, 1, 1), 
								Quaternion(Vector3f(0, 1, 0), 45));
	
	mp_effect->setWVP(projViewMatrix*transMatrix);
	mp_effect->setHasBones(0);

	m_meshPtr->render(*mp_effect);
}
};
