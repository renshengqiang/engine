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

	typedef std::deque<unsigned long>	TimeQueue;//��Ż�ȡ��ʱ��
	typedef TimeQueue::iterator			TimeQueueIter;
	
	Root();
	~Root();

	RenderWindow* createRenderWindow(int width, int height, bool fullScreen);
	
	SceneManager* createSceneManager(SceneManagerType type, const std::string& name);
	void destroySceneManager(const std::string& name);

	void addFrameListener(FrameListener *pListener);
	void removeFrameListener(FrameListener *pListener);
	
	/**	�������е�FrameListener,ִ��frameRenderingQueued����
		����false������Ⱦ��ѭ������Ⱦ�߳���ֹ
	*/
	bool _fireFrameQueued();
	
	/**	��ȡ��֮֡����¼���
		�¼�����һ���趨��ʱ���ڽ�����ƽ������֤ʱ����Ϊƽ��
	*/
	float _calcFrameTime();

	/**	�趨����ʱ����ƽ������ʱ��
		������m_timerQueue�����е�ʱ��֮��Ĳ�ֵ������m_frameSmoothingTime֮��
		������֮֡��Ĳ�ֵ����ͨ�����һ֡���¼���ȥ��ʼһ֡��ʱ���ٳ���֡���õ�
		���Ǳ��뱣֤m_timerQueue����������֡	
		����Ӧ�ü������_calcFrameTime
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
