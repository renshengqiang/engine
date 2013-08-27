#ifndef _ENGINE_SCENE_MANAGER_H
#define _ENGINE_SCENE_MANAGER_H
#include <EngineRenderQueue.h>
#include <EngineAnimationState.h>
#include <vector>
#include <map>
#include <string>

namespace Engine
{
class Vector3f;
class Camera;
class Entity;
class SceneNode;
class RaySceneQuery;
class Ray;
class Animation;

class  SceneManager
{
public:
	typedef std::map<std::string, Animation*> AnimationList;
	typedef AnimationList::iterator AnimationIterator;
	typedef AnimationList::const_iterator AnimationConstIterator;
	
	SceneManager();
	virtual ~SceneManager();
	
	virtual Camera *createCamera(const Vector3f& pos, const Vector3f &target, const Vector3f &up);
	virtual RaySceneQuery *createRayQuery(const Ray &ray);
	
	SceneNode *getRootNode();
	
	Animation *createAnimation(const std::string &name, float length);
		
	/** Looks up an Animation object previously created with createAnimation
	@note return NULL if the named animation does not exist
	*/
	Animation *getAnimation(const std::string &name) const;
	
	/**
		returns whether an animation with the given name exists
	*/
	bool hasAnimation(const std::string &name) const;
	
	/**
		destroy an animation with the given name
	*/
	void destroyAnimation(const std::string &name);
	
	/** 
		Removes all animations created using this SceneManager
	*/
	void destroyAllAnimations(void);

	/**
		Animation�����������ƽڵ㶯����
		����Ϊ����Animation��������Animation�Ǳ����
		Animation��AnimationState��һһ��Ӧ��
	*/
	AnimationState *createAnimationState(const std::string &name);
	AnimationState *getAnimationState(const std::string name);
	bool hasAnimationState(const std::string &name) const;
	void destroyAnimationState(const std::string &name);
	void destroyAllAnimationStates(void);

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
	AnimationList m_animationList;
	AnimationStateSet m_animationStateSet;
};
}
#endif
