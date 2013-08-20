#ifndef _UTIL_MUTEX_LOCK_H
#define _UTIL_MUTEX_LOCK_H
#include <pthread.h>
#include <assert.h>

namespace Util
{
class MutexLock
{
public:
	inline MutexLock()
	{
		int ret;
		ret = pthread_mutex_init(&m_mutex, NULL);
		assert(ret == 0); (void) ret;
	}
	inline ~MutexLock()
	{
		int ret;
		ret = pthread_mutex_destroy(&m_mutex);
		assert(ret == 0); (void) ret;
	}
	inline void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}
	inline void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}
	pthread_mutex_t* getPthreadMutex() /* non-const */
	{
		return &m_mutex;
	}
private:
	pthread_mutex_t m_mutex;
};

class MutexLockGuard
{
public:
	explicit inline MutexLockGuard(MutexLock& mutexLock):
		m_mutexLock(mutexLock)
	{
		m_mutexLock.lock();
	}

	inline ~MutexLockGuard()
	{
		m_mutexLock.unlock();
	}
private:
	MutexLock& m_mutexLock;
};
}

#endif
