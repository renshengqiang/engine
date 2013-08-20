#include "UtilThread.h"
#include <assert.h>
#include <stdio.h>

namespace Util
{
Thread::Thread(const ThreadFunc &func):
	m_started(false),
	m_joined(false),
	m_pthreadId(0),
	m_func(func)
{
	m_func = func;
}
	
Thread::~Thread()
{
	if(m_started && !m_joined)
	{
		pthread_detach(m_pthreadId);
		m_pthreadId = 0;
	}
}

void Thread::start(void *p)
{
	assert(!m_started);
	m_started = true;
	if (pthread_create(&m_pthreadId, NULL, m_func, p))
	{
		m_started = false;
		fprintf(stderr, "ERROR Thread::start: create thread error\n");
	}
}

void Thread::cancel()
{
	assert(m_started);
	if(pthread_cancel(m_pthreadId))
	{
		fprintf(stderr, "ERROR Thread::cancel: thread cancel fail\n");
	}
}

int Thread::join()
{
	assert(m_started);
	assert(!m_joined);
	m_joined = true;
	return pthread_join(m_pthreadId, NULL);
}
}