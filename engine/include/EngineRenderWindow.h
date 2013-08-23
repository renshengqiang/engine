#ifndef _ENGINE_RENDER_WINDOW_H
#define _ENGINE_RENDER_WINDOW_H
#include <string>

namespace Engine
{
class RenderWindow
{
public:
	inline RenderWindow(const char *title, int width, int height):
		m_title(title), m_width(width), m_height(height)
	{
	}
	inline int width(void){ return m_width;}
	inline int height(void){ return m_height;}
	bool initWindow(void);
	void destroy(void);
	void swapBuffer(void);
	
private:
	std::string m_title;
	int m_width, m_height;
};
}

#endif
