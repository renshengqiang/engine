#ifndef _ENGINE_SUB_ENTITY_H
#define _ENGINE_SUB_ENTITY_H
#include <EngineRenderable.h>
#include <tr1/memory>

namespace Engine
{
class Entity;
class SubMesh;
class Effect;

//SubEntity��Ҫ����������Ⱦ�߳�ʹ�õ�
//���������߳���ɾ����Entityʱ������Entity���汣�����shared_ptr<SubEntity>Ҳ���ᵼ����Ⱦ�̶߳δ���
//SubEntity���ܻ�ʹ�õ�Entity�еĹ������󣬺�����Ҫ��ϸ��������ܹ������̰߳�ȫ�ķ���
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
