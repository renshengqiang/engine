#ifndef _DEMO_APP_H
#define _DEMO_APP_H
#include <EngineFrameListener.h>
#include <EngineRoot.h>
#include <EngineSceneManager.h>
#include <SDL/SDL.h>

using namespace Engine;

class DemoApp:public FrameListener
{
public:
	DemoApp();
	~DemoApp();

	void createScene();
	void run();

	virtual bool  frameRenderingQueued(float timeSinceLastFrame);
private:
	void _processWindowEvents();
	void _processKeyboardEvents(const SDL_keysym &keysym);
	
private:
	bool m_quit;
	Root *mp_root;
	RenderWindow *mp_renderingWindow;
	SceneManager *mp_sceneManager;
};

#endif
