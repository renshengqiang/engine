#ifndef _UTIL_THREAD_H
#define _UTIL_THREAD_H
#include <pthread.h>

namespace Util
{
class Thread
{
public:
	typedef void * (*ThreadFunc)(void *p);

	explicit Thread(const ThreadFunc&);
	~Thread();

	void start(void *p);
	void cancel();
	 int join(); // return pthread_join()
	
private:
	bool m_started;
	bool m_joined;
	pthread_t m_pthreadId;
	ThreadFunc m_func;	
};
}

#endif