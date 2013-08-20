#ifndef _UTIL_TIMER_H
#define _UTIL_TIMER_H
#include <sys/time.h>
namespace Util
{
class Timer
{
public:
	Timer();
	void reset();
	//��ȡ�ӳ������������ڵ�ʱ�䣬�����
	unsigned long getMilliSeconds();
	//��ȡ�ӳ������������ڵ�ʱ�䣬΢���
	unsigned long getMicroSeconds();
private:
	struct timeval m_start;
};
}

#endif
