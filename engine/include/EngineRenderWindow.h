#ifndef _ENGINE_RENDER_WINDOW_H
#define _ENGINE_RENDER_WINDOW_H

namespace Engine
{
class RenderWindow
{
public:
	inline RenderWindow(int width, int height):
		m_width(width), m_height(height)
	{
	}
	inline int width(void){ return m_width;}
	inline int height(void){ return m_height;}
	bool initWindow(void);
	void destroy(void);
	void swapBuffer(void);
	
private:
	int m_width, m_height;
};
}

#endif
