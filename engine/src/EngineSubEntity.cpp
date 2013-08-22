#include "EngineSubEntity.h"
#include "EngineEntity.h"
#include "EngineSubMesh.h"
#include "EngineSimpleMeshEffect.h"

namespace Engine
{
SubEntity::SubEntity(Entity *parentEntity, SubMesh *subMesh)
{
	mp_parentEntity = parentEntity;
	mp_subMesh = subMesh;
}

SubEntity::~SubEntity()
{
	if(mp_subMesh)
	{
		delete mp_subMesh;
	}
}

Effect *SubEntity::getEffect()
{
	//FIXME:�ȼ�����EffectManager���EffectManager��ȥȡEffect
	return NULL;
}

void SubEntity::render()
{
	//FIXME: subMesh->getVerci
	//Draw(...)
}
}