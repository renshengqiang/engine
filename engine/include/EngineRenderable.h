#ifndef _ENGINE_RENDERABLE_H
#define _ENGINE_RENDERABLE_H
#include <string>
#include <tr1/memory>

namespace Engine
{
class Effect;
class Matrix4f;
class Renderable
{
public:
	virtual ~Renderable(){}
	virtual Effect *getEffect() const { return NULL;}
	virtual void update(){}
	virtual void render(const Matrix4f &trans) const = 0;
protected:
	std::string m_type;
};
typedef std::tr1::shared_ptr<Renderable> RenderablePtr;
}

#endif