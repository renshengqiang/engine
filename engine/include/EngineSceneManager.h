#ifndef _ENGINE_SCENE_MANAGER_H
#define _ENGINE_SCENE_MANAGER_H

namespace Engine
{
class Vector3f;
class SceneNode;
class Camera;
class Mesh;
class SimpleMeshEffect;

class  SceneManager
{
public:
	SceneManager();
	
	Camera* createCamera(const Vector3f& pos, const Vector3f target, const Vector3f up);
	void render();
private:
	Camera	*mp_camera;
	Mesh	*mp_mesh;
	bool		m_initialised;
	SimpleMeshEffect *mp_effect;
};
}
#endif
