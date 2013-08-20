#include "EngineSceneManager.h"
#include "EngineMesh.h"
#include "EngineSimpleMeshEffect.h"
#include "EngineMatrix.h"
#include "EngineCamera.h"
#include <stdio.h>

namespace Engine
{
SceneManager::SceneManager():
	mp_camera(NULL),
	mp_mesh(NULL),
	m_initialised(false)
{
	mp_effect = new SimpleMeshEffect("./media/effects/SimpleMeshEffect.vs", "./media/effects/SimpleMeshEffect.fs");
	mp_mesh = new Mesh("./media/models/phoenix_ugv.md2");
	mp_mesh->load();
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
	if(m_initialised == false)
	{
		mp_effect->init();
		mp_effect->enable();
		m_initialised = true;
	}
	Matrix4f projViewMatrix = mp_camera->getProjViewMatrix();
/*
	printf("begin %f %f %f %f\n \
	    %f %f %f %f\n \
	    %f %f %f %f\n \
	    %f %f %f %f\n", 
	    projViewMatrix[0][0], projViewMatrix[0][1], projViewMatrix[0][2], projViewMatrix[0][3],
	    projViewMatrix[1][0], projViewMatrix[1][1], projViewMatrix[1][2], projViewMatrix[1][3],
	    projViewMatrix[2][0], projViewMatrix[2][1], projViewMatrix[2][2], projViewMatrix[2][3],
	    projViewMatrix[3][0], projViewMatrix[3][1], projViewMatrix[3][2], projViewMatrix[3][3]);
*/
	mp_effect->setWVP(projViewMatrix);
	mp_effect->setHasBones(0);
	mp_mesh->render(*mp_effect);
}
};
