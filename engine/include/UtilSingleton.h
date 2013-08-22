#ifndef _UTIL_SINGLETON_H
#define _UTIL_SINGLETON_H
#include <assert.h>

namespace Util
{
template <typename Type>
class Singleton
{
public:
	Singleton()
	{
		assert(sp_instance == 0);
		sp_instance = static_cast<Type*>(this);
	}
	~Singleton()
	{
		assert(sp_instance);
		sp_instance = 0;
	}
	static Type *getSingletonPtr()
	{
		assert(sp_instance);
		return sp_instance;
	}
	static Type& getSingleton()
	{
		assert(sp_instance);
		return *sp_instance;
	}
protected:
	static Type *sp_instance;

private:
	//�����������������������������Բ������¶���
	Singleton(const Singleton<Type>&);
	Singleton& operator=(const Singleton<Type>&);
};
}

#endif
