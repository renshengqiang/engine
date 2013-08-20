#ifndef _UTIL_CONDITION_H
#define _UTIL_CONDTION_H
#include <UtilMutexLock.h>
#include <pthread.h>
#include <assert.h>

namespace Util
{
class Condition
{
public:
	inline Condition(MutexLock& mutex):
		m_lock(mutex)
	{
		int ret;
		ret = pthread_cond_init(&m_cond, NULL);
		assert(ret == 0);		
	}
	inline ~Condition()
	{
		int ret;
		ret = pthread_cond_destroy(&m_cond);
		assert(ret == 0);
	}
	inline void wait()
	{
		pthread_cond_wait(&m_cond, m_lock.getPthreadMutex());
	}
	inline void notify()
	{
		pthread_cond_signal(&m_cond);
	}
	inline void notifyAll()
	{
		pthread_cond_broadcast(&m_cond);
	}
private:
	MutexLock&		m_lock;
	pthread_cond_t	m_cond;
};
}
#endif
