#ifndef _ENGINE_SUB_ENTITY_H
#define _ENGINE_SUB_ENTITY_H
#include <EngineRenderable.h>
#include <tr1/memory>

namespace Engine
{
class Entity;
class SubMesh;
class Effect;

//SubEntity主要是用来给渲染线程使用的
//这样当主线程中删除了Entity时，由于Entity里面保存的是shared_ptr<SubEntity>也不会导致渲染线程段错误
//SubEntity可能会使用到Entity中的骨骼矩阵，后面需要仔细想想如何能够做到线程安全的访问
class SubEntity: public Renderable
{
public:
	SubEntity(Entity *parentEntity, SubMesh *subMesh);
	~SubEntity();
	
	Effect *getEffect();
	void render();
private:
	Entity	*mp_parentEntity;
	SubMesh	*mp_subMesh;
};
typedef std::tr1::shared_ptr<SubEntity> SubEntityPtr;
}

#endif
