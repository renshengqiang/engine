#include "EngineSceneManager.h"
#include "EngineMatrix.h"
#include "EngineQuaternion.h"
#include "EngineCamera.h"
#include "EngineSceneNode.h"
#include "EngineEntity.h"
#include "EngineRaySceneQuery.h"
#include "EngineRay.h"
#include "EngineAnimation.h"
#include "EngineAnimationTrack.h"
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

Camera* SceneManager::createCamera(const Vector3f &pos,const Vector3f &target,const Vector3f &up)
{
	if(mp_camera)
	{
		delete mp_camera;
	}
	mp_camera = new Camera(pos, target, up);
	return mp_camera;
}

RaySceneQuery* SceneManager::createRayQuery(const Ray &ray)
{
	RaySceneQuery *pQuery = new RaySceneQuery(this);
	pQuery->setRay(ray);
	return pQuery;
}

SceneNode *SceneManager::getRootNode()
{
	return mp_rootNode;
}

bool SceneManager::hasAnimation(const  std::string & name) const
{
	return m_animationList.find(name) != m_animationList.end();
}

Animation *SceneManager::getAnimation(const std::string &name) const
{
	AnimationConstIterator iter = m_animationList.find(name);
	if(iter != m_animationList.end())
	{
		return iter->second;
	}
	return NULL;
}

Animation *SceneManager::createAnimation(const  std::string & name,float length)
{
	if(hasAnimation(name))
	{
		fprintf(stderr, "ERROR SceneManager::CreateAnimation : animation named %s already exists\n", name.c_str());
		return NULL;
	}
	Animation *animation = new Animation(name, length);
	m_animationList.insert(std::make_pair(name, animation));
	return animation;
}

void SceneManager::destroyAnimation(const  std::string & name)
{
	AnimationIterator iter = m_animationList.find(name);

	if(iter != m_animationList.end())
	{
		delete iter->second;
		m_animationList.erase(iter);
	}
	else
	{
		fprintf(stderr, "SceneManager::DestroyAnimation : Error Destroy Node Animation, animation named %s already exists\n", name.c_str());
	}
	return ;
}

void SceneManager::destroyAllAnimations(void)
{
	for(AnimationIterator iter = m_animationList.begin(); iter != m_animationList.end(); ++iter)
	{
		delete iter->second;
	}
	m_animationList.clear();
	return;
}

bool SceneManager::hasAnimationState(const std::string &name) const
{
	return m_animationStateSet.hasAnimationState(name);
}

AnimationState *SceneManager::getAnimationState(const  std::string name)
{
	return m_animationStateSet.getAnimationState(name);
}

AnimationState *SceneManager::createAnimationState(const  std::string &name)
{
	if(hasAnimationState(name))
	{
		fprintf(stderr, "ERROR SceneManager::HasAnimationState : AnimationState named %s already exists\n", name.c_str());
		return NULL;
	}
	Animation *animation = getAnimation(name);
	if(animation == NULL)
	{
		fprintf(stderr, "ERROR SceneManager::CreateAnimationState : Animation named %s does not exist\n", name.c_str());
		return NULL;
	}

	return m_animationStateSet.createAnimationState(name, animation->getLength());
}

void SceneManager::destroyAnimationState(const  std::string &name)
{
	m_animationStateSet.removeAnimationState(name);
}

void SceneManager::destroyAllAnimationStates(void)
{
	m_animationStateSet.removeAllAnimationStates();
}

void SceneManager::clearScene()
{
	mp_rootNode->detachAllChildren();
	mp_rootNode->detachObject();
}

void SceneManager::_applySceneAnimations()
{
	AnimationStateSet::EnabledAnimationStateIterator iter = m_animationStateSet._animationIteratorBegin();

	for(; iter!=m_animationStateSet._animationIeratorEnd(); ++iter)
	{
		AnimationState *animationState = *iter;
		Animation *animation = getAnimation(animationState->getName());

		//reset
		for(Animation::NodeTrackIterator iter1 = animation->_nodeTrackBegin(); 
			iter1 != animation->_nodeTrackEnd(); ++iter1)
		{
			NodeAnimationTrack *track = iter1->second;
			Node *node = track->getAssociatedNode();
			node->resetToInitialState();
		}
			
		//use animation
		animation->apply(animationState->getTimePosition());
	}
}

void SceneManager::_updateSceneGraph()
{
	mp_rootNode->_update(true, false);
}

void SceneManager::_findVisibleObjects(Camera *pCamera, RenderQueue &queue)
{
	mp_rootNode->_findVisibleObjects(pCamera, queue);
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

}
