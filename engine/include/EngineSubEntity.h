#ifndef _ENGINE_SUB_ENTITY_H
#define _ENGINE_SUB_ENTITY_H
#include <EngineRenderable.h>

namespace Engine
{
class Entity;
class SubMesh;
class Effect;
class Matrix4f;

//SubEntity主要是用来给渲染线程使用的
//这样当主线程中删除了Entity时，由于Entity里面保存的是shared_ptr<SubEntity>也不会导致渲染线程段错误
//SubEntity可能会使用到Entity中的骨骼矩阵，后面需要仔细想想如何能够做到线程安全的访问
class SubEntity: public Renderable
{
public:
	SubEntity(Entity *parentEntity, SubMesh *subMesh);
	~SubEntity();
	
	virtual Effect *getEffect() const;
	virtual void render(const Matrix4f &trans) const;
private:
	Entity	*mp_parentEntity;
	SubMesh	*mp_subMesh;
};
}

#endif
