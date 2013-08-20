#include "UtilTimer.h"
#include <stddef.h>

namespace Util
{
Timer::Timer()
{
	reset();
}
void Timer::reset()
{
	gettimeofday(&m_start, NULL);
}
unsigned long Timer::getMilliSeconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-m_start.tv_sec)*1000+(now.tv_usec-m_start.tv_usec)/1000;
}
unsigned long Timer::getMicroSeconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-m_start.tv_sec)*1000000+(now.tv_usec-m_start.tv_usec);
}
}
