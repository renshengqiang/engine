#ifndef _ENGINE_SUB_ENTITY_H
#define _ENGINE_SUB_ENTITY_H
#include <EngineRenderable.h>

namespace Engine
{
class Entity;
class SubMesh;
class Effect;
class Matrix4f;

//SubEntity��Ҫ����������Ⱦ�߳�ʹ�õ�
//���������߳���ɾ����Entityʱ������Entity���汣�����shared_ptr<SubEntity>Ҳ���ᵼ����Ⱦ�̶߳δ���
//SubEntity���ܻ�ʹ�õ�Entity�еĹ������󣬺�����Ҫ��ϸ��������ܹ������̰߳�ȫ�ķ���
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
