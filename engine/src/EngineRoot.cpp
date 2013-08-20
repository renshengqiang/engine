#include "EngineRoot.h"
#include "EngineRenderWindow.h"
#include "EngineSceneManager.h"
#include "EngineRenderOp.h"
#include "EngineFrameListener.h"
#include <unistd.h>//sleep
#include <stdio.h>

namespace Engine
{
Root::Root():
	mp_renderWindow(NULL),
	m_renderingThread(renderThreadFunc),
	m_windowInitialised(false),
	m_windowCondtion(m_windowMutex),
	m_renderQueueFull(false),
	m_renderQueueFullCond(m_renderQueueFullMutex),
	m_renderingEmptyCond(m_renderQueueFullMutex),
	m_frameSmoothingTime(0)
{
}	
	
Root::~Root()
{
	//destroy all SceneManagers
	for(SceneManagerIter iter = m_sceneManagerMap.begin(); 
			iter != m_sceneManagerMap.end(); ++iter)
	{
		delete iter->second;
	}
	m_sceneManagerMap.clear();
}

RenderWindow* Root::createRenderWindow(int width, int height, bool fullScreen)
{
	mp_renderWindow = new RenderWindow(width, height);

	//创建渲染线程并等待窗口初始化完毕
	m_renderingThread.start(this);
	m_windowMutex.lock();
	while(m_windowInitialised == false) m_windowCondtion.wait();
	m_windowMutex.unlock();
		
	return mp_renderWindow;
}

SceneManager* Root::createSceneManager(SceneManagerType type, const std::string& name)
{
	SceneManager *pManager = NULL;

	switch(type)
	{
	case GENERAL_MANAGER:
		pManager = new SceneManager();
		break;
	case OCTREE_MANAGER:
		break;
	}
	if(pManager)
	{
		m_sceneManagerMap.insert(std::make_pair(name, pManager));
	}
	return pManager;
}

void  Root::destroySceneManager(const std::string& name)
{
	SceneManagerIter iter = m_sceneManagerMap.find(name);

	if(iter == m_sceneManagerMap.end())
	{
		fprintf(stderr, "Engine::Root::destroySceneManager: SceneManager '%s' not found\n", name.c_str());
	}
	else
	{
		SceneManager *pManager = iter->second;
		m_sceneManagerMap.erase(iter);
		delete pManager;
	}
}

void Root::addFrameListener(FrameListener *pListener)
{
	FrameListenerIter iter = m_frameListenerSet.find(pListener);
	if(iter == m_frameListenerSet.end() && pListener)
	{
		m_frameListenerSet.insert(pListener);
	}
	else if(NULL == pListener)
	{
		fprintf(stderr, "WARNING Root::addFrameListener: invalid listener\n");
	}
	else
	{
		fprintf(stderr, "WARNING Root::addFrameListener: listener haved added more than once\n");
	}
}

void Root::removeFrameListener(FrameListener *pListener)
{
	FrameListenerIter iter = m_frameListenerSet.find(pListener);
	if(iter != m_frameListenerSet.end())
	{
		m_frameListenerSet.erase(iter);
	}
	else
	{
		fprintf(stderr, "WARNING Root::removeFrameListener: listener to be removed is not exist");
	}
}

bool  Root::_fireFrameQueued()
{
	float timeSinceLastFrame = _calcFrameTime();
	for(FrameListenerIter it = m_frameListenerSet.begin(); it != m_frameListenerSet.end(); ++it)
	{
		if(!((*it)->frameRenderingQueued(timeSinceLastFrame)))
			return false;
	}
	return true;
}

float Root::_calcFrameTime()
{	
	unsigned long now = m_timer.getMilliSeconds();
	m_timeQueue.push_back(now);
	if(m_timeQueue.size() == 1) return 0;

	// Times up to mFrameSmoothingTime seconds old should be kept
	unsigned long discardThreshold = static_cast<unsigned long>(m_frameSmoothingTime * 1000.0f);

	TimeQueueIter it = m_timeQueue.begin(), end = m_timeQueue.end() - 2;
	while(it != end)
	{
		if (now - *it > discardThreshold)
			++it;
		else
			break;
	}
	m_timeQueue.erase(m_timeQueue.begin(), it);
	return float(m_timeQueue.back() - m_timeQueue.front()) / ((m_timeQueue.size()-1) * 1000); 
}

void Root::startRendering()
{
	assert(mp_renderWindow != NULL);
	while(true)
	{
		if(!_fireFrameQueued())
		{	
			m_renderingThread.cancel();
			break;
		}
		m_renderQueueFullMutex.lock();
		while(m_renderQueueFull == true)
			m_renderingEmptyCond.wait();
		m_renderQueueFull = true;
		m_renderQueueFullMutex.unlock();
		m_renderQueueFullCond.notify();
	}
}

void* Root::renderThreadFunc(void *p)
{
	Root *pRoot = static_cast<Root*>(p);
	RenderWindow *pWindow = pRoot->mp_renderWindow;
	assert(pWindow != NULL);

	bool ret = pWindow->initWindow();
	assert(ret == true);
	
	pRoot->m_windowMutex.lock();
	pRoot->m_windowInitialised = true;
	pRoot->m_windowMutex.unlock();
	pRoot->m_windowCondtion.notify();

	while(true)
	{
		pRoot->m_renderQueueFullMutex.lock();
		while(pRoot->m_renderQueueFull == false)
			pRoot->m_renderQueueFullCond.wait();
		pRoot->m_renderQueueFull = false;
		pRoot->m_renderQueueFullMutex.unlock();
		pRoot->m_renderingEmptyCond.notify();
		
		ClearBuffer();

		for(SceneManagerIter it = pRoot->m_sceneManagerMap.begin(); it!=pRoot->m_sceneManagerMap.end(); ++it)
		{
			it->second->render();
		}
		
		pWindow->swapBuffer();
	}
}

}
