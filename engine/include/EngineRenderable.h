#ifndef _ENGINE_RENDERABLE_H
#define _ENGINE_RENDERABLE_H

namespace Engine
{
class Effect;
class Renderable
{
public:
	virtual Effect *getEffect() = 0;
	virtual void update(){}
	virtual void render() = 0;
protected:
	int type;
};
}

#endif