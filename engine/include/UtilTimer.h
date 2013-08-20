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
	//获取从程序启动到现在的时间，毫秒计
	unsigned long getMilliSeconds();
	//获取从程序启动到现在的时间，微秒计
	unsigned long getMicroSeconds();
private:
	struct timeval m_start;
};
}

#endif
