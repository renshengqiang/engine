#ifndef _ENGINE_RENDER_QUEUE_H
#define _ENGINE_RENDER_QUEUE_H
#include <EngineMatrix.h>
#include <EngineRenderable.h>
#include <vector>

namespace Engine
{
struct RenderItem
{
	Matrix4f	transformation;
	RenderablePtr	pRenderable;
};

class RenderQueue
{
public:
	typedef std::vector<RenderItem>::iterator Iterator;

	Iterator begin(){ return m_renderItemVec.begin();}
	Iterator end(){ return m_renderItemVec.end();}
	int size(){ return m_renderItemVec.size(); }

	void addItem(const Matrix4f &trans, RenderablePtr &renderable)
	{
		struct RenderItem item;

		item.transformation = trans;
		item.pRenderable = renderable;
		m_renderItemVec.push_back(item);
	}
	void clear() { m_renderItemVec.clear();}
	
private:
	std::vector<RenderItem> m_renderItemVec;
};
}

#endif
