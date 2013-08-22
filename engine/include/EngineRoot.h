#ifndef _ENGINE_ROOT_H
#define _ENGINE_ROOT_H
#include <UtilThread.h>
#include <UtilMutexLock.h>
#include <UtilCondition.h>
#include <UtilTimer.h>
#include <UtilSingleton.h>
#include <map>
#include <set>
#include <deque>
#include <string>

namespace Engine
{
class SceneManager;
class RenderWindow;
class FrameListener;
class MeshManager;
class EffectManager;
class Root: public Util::Singleton<Root>
{
public:
	/// Bitmask containing scene types
	enum SceneManagerType
	{
		GENERAL_MANAGER = 0x1,
		OCTREE_MANAGER = 0x2
	};

	typedef std::map<std::string, SceneManager*>	SceneManagerMap;
	typedef SceneManagerMap::iterator 			SceneManagerIter;

	typedef std::set<FrameListener*>		FrameListenerSet;
	typedef FrameListenerSet::iterator		FrameListenerIter;

	typedef std::deque<unsigned long>	TimeQueue;//存放获取的时间
	typedef TimeQueue::iterator			TimeQueueIter;
	
	Root();
	~Root();

	RenderWindow* createRenderWindow(int width, int height, bool fullScreen);
	
	SceneManager* createSceneManager(SceneManagerType type, const std::string& name);
	void destroySceneManager(const std::string& name);

	void addFrameListener(FrameListener *pListener);
	void removeFrameListener(FrameListener *pListener);
	
	/**	遍历所有的FrameListener,执行frameRenderingQueued函数
		返回false导致渲染主循环和渲染线程终止
	*/
	bool _fireFrameQueued();
	
	/**	获取两帧之间的事件差
		事件差在一个设定的时间内进行了平均，保证时间差较为平均
	*/
	float _calcFrameTime();

	/**	设定计算时间差的平滑过渡时间
		保存在m_timerQueue队列中的时间之间的差值保持在m_frameSmoothingTime之间
		计算两帧之间的差值，是通过最后一帧的事件减去开始一帧的时间再除以帧数得到
		但是必须保证m_timerQueue中至少有两帧	
		具体应用及计算见_calcFrameTime
	*/
	void setFrameSmoothingPeriod(float period) { m_frameSmoothingTime = period; }
	/** Gets the period over which OGRE smooths out fluctuations in frame times. */
	float getFrameSmoothingPeriod(void) const { return m_frameSmoothingTime; }
	
	void startRendering();
	
	static void* renderThreadFunc(void *);
private:
	RenderWindow		*mp_renderWindow;
		
	Util::Thread		m_renderingThread;
	
	bool				m_windowInitialised;
	Util::MutexLock		m_windowMutex;
	Util::Condition		m_windowCondtion;
	
	bool				m_renderQueueFull;
	Util::MutexLock		m_renderQueueFullMutex;
	Util::Condition		m_renderQueueFullCond;
	Util::Condition		m_renderingEmptyCond;
	
	SceneManagerMap		m_sceneManagerMap;
	FrameListenerSet	m_frameListenerSet;
	float				m_frameSmoothingTime;
	TimeQueue			m_timeQueue;
	Util::Timer			m_timer;

	MeshManager			*mp_meshManager;
	EffectManager		*mp_effectManager;
};
}

#endif
