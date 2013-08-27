#include "DemoApp.h"
#include <EngineVector.h>
#include <EngineEntity.h>
#include <EngineSceneNode.h>
#include <EngineRay.h>
#include <EngineKeyFrame.h>
#include <EngineAnimationTrack.h>
#include <stdio.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

DemoApp::DemoApp():
	m_quit(false),
	mp_root(NULL),
	mp_renderingWindow(NULL),
	mp_sceneManager(NULL),
	mp_nodeState1(NULL)
{
	mp_root = new Engine::Root();
	mp_renderingWindow = mp_root->createRenderWindow("Demo", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	mp_root->addFrameListener(this);
	mp_sceneManager = mp_root->createSceneManager(Engine::Root::GENERAL_MANAGER, "g-manager");
	mp_camera = mp_sceneManager->createCamera(Engine::Vector3f(0.0f, 0.0f, 0.0f), Engine::Vector3f(0.0f, 0.0f, -1.0f), Engine::Vector3f(0.0f, 1.0f, 0.0f));
}
	
DemoApp::~DemoApp()
{
}

void DemoApp::createScene()
{
	mp_camera->translate(Vector3f(0, 0, 200));
	Engine::Entity *pEntity = new Engine::Entity("./media/models/phoenix_ugv.md2");
	//mp_sceneManager->getRootNode()->attachObject(pEntity);
	Engine::SceneNode *pNode = mp_sceneManager->getRootNode()->createChildSceneNode("boblamp");
	pEntity = new Engine::Entity("./media/models/boblampclean.md5mesh");
	pNode->attachObject(pEntity);
	pNode->translate(100, 0, -100);
	pNode->rotate(Engine::Vector3f(1, 0, 0), -90);

	/// create a ray scene query
	mp_raySceneQuery = mp_sceneManager->createRayQuery(Engine::Ray());

	/// create animations
	Engine::Animation *animation = mp_sceneManager->createAnimation("transAnim1-1",5.792);
	//animation->setRotationInterpolationMode(Animation::RIM_SPHERICAL);
	Engine::NodeAnimationTrack *track = animation->createNodeTrack(0, pNode);
	Engine::TransformKeyFrame *keyFrame = track->createNodeKeyFrame(1.448);
	keyFrame->setTranslate(Vector3f(-100, 0, 0));
	keyFrame = track->createNodeKeyFrame(2.896);
	keyFrame->setTranslate(Vector3f(-100, 0, -100));
	keyFrame->setRotation(Quaternion(Vector3f(0,1,0),-90));
	keyFrame->setScale(Vector3f(0.5 , 0.5, 0.5));
	keyFrame = track->createNodeKeyFrame(5.792);
	keyFrame->setTranslate(Vector3f(0,0,0));
	
	pNode->setInitialState();
	mp_nodeState1 = mp_sceneManager->createAnimationState("transAnim1-1");
	mp_nodeState1->setEnabled(true);
	mp_nodeState1->setLoop(true);
}

void DemoApp::run()
{
	createScene();
	if(mp_root && mp_renderingWindow)
	{
		mp_root->startRendering();
	}
}

void DemoApp::_processWindowEvents()
{
	SDL_Event event; 
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:		//¹Ø±Õ´°¿Ú
			m_quit = true;
			break;
		case SDL_KEYDOWN:
			_processKeyboardEvents(event.key.keysym);
			break;
		case	SDL_MOUSEBUTTONDOWN:
			{
			Engine::Ray mouseRay = mp_camera->getCameraToViewportRay((float)event.button.x/WINDOW_WIDTH, (float)event.button.y/WINDOW_HEIGHT);
			mp_raySceneQuery->setRay(mouseRay);
			Engine::RaySceneQueryResult &result = mp_raySceneQuery->execute();
			
			Engine::RaySceneQueryResult::iterator iter;
			for(iter=result.begin(); iter!=result.end(); iter++)
			{
				SceneNode *pNode = iter->pMoveable->getParentSceneNode();
				pNode->translate(Engine::Vector3f(0, 0, -100));
			}
			}
			break;
		case	SDL_MOUSEBUTTONUP:
			{
			Engine::Ray mouseRay = mp_camera->getCameraToViewportRay((float)event.button.x/WINDOW_WIDTH, (float)event.button.y/WINDOW_HEIGHT);
			mp_raySceneQuery->setRay(mouseRay);
			Engine::RaySceneQueryResult &result = mp_raySceneQuery->execute();
			
			Engine::RaySceneQueryResult::iterator iter;
			for(iter=result.begin(); iter!=result.end(); iter++)
			{
				SceneNode *pNode = iter->pMoveable->getParentSceneNode();
				pNode->translate(Engine::Vector3f(0, 0, 100));
			}
			}
			break;
		}	
	}
}

void DemoApp::_processKeyboardEvents(const SDL_keysym &keysym)
{
	switch(keysym.sym)
	{
		case SDLK_ESCAPE:
			m_quit = true;
			break;
		default:
			break;
	}
}

bool  DemoApp::frameRenderingQueued(float timeSinceLastFrame)
{	
	if(mp_nodeState1) 
	{
		mp_nodeState1->addTime(timeSinceLastFrame);
	}
	_processWindowEvents();
	if(m_quit) return false;
	return true;
}

