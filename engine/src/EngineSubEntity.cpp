#include "EngineSubEntity.h"
#include "EngineEntity.h"
#include "EngineSubMesh.h"
#include "EngineSimpleMeshEffect.h"
#include "EngineEffectManager.h"

namespace Engine
{
SubEntity::SubEntity(Entity *parentEntity, SubMesh *subMesh)
{
	mp_parentEntity = parentEntity;
	mp_subMesh = subMesh;
	m_type = "SubEntity";
}

SubEntity::~SubEntity()
{
	if(mp_subMesh)
	{
		delete mp_subMesh;
	}
}

Effect *SubEntity::getEffect() const
{
	EffectManager &manager = EffectManager::getSingleton();
	return manager.getEffec(m_type);
}

void SubEntity::render(const Matrix4f& trans) const
{
	SimpleMeshEffect *pEffect = dynamic_cast<SimpleMeshEffect*>(getEffect());
	if(pEffect)
	{
		pEffect->enable();
		
		pEffect->setWVP(trans);
		pEffect->setHasBones(0);

		DrawSimpleMesh(mp_subMesh->getIndexObject(), 
					    mp_subMesh->getVertexObject(), 
					    mp_subMesh->getPixelObject(),
					    *pEffect);
	}
}
}