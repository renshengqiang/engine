#include "EngineEffectManager.h"
#include "EngineSimpleMeshEffect.h"
#include "EngineParticleSystemEffect.h"
#include "EngineEffect.h"
#include <stdio.h>

namespace Engine
{
template<> EffectManager* Util::Singleton<EffectManager>::sp_instance = 0;

EffectManager::EffectManager()
{
	addEffect("SubEntity", "./media/effects/SimpleMeshEffect.vs", "./media/effects/SimpleMeshEffect.fs");
	//addEffect("ParticleSystem", "./media/effects/ParticleEffect.vs", "./media/effects/ParticleEffect.fs");
}

EffectManager::~EffectManager()
{
	for(EffectIter it = m_effectMap.begin(); it != m_effectMap.end(); ++it)
	{
		if(it->second)
		{
			delete it->second;
		}
	}
}

void EffectManager::addEffect(const std::string &type, const char *vsFile, const char *fsFile)
{
	EffectIter it = m_effectMap.find(type);
	if(it != m_effectMap.end())
	{
		fprintf(stderr, "WARNING EffectManager::addEffect: effect named '%s' already exitst\n", type.c_str());
	}
	else
	{
		Effect *pEffect;
		if(type == "SubEntity")
			pEffect = new SimpleMeshEffect(vsFile, fsFile);
		else if(type == "ParticleSystem")
			pEffect = new ParticleEffect(vsFile, vsFile);
		else
		{
			fprintf(stderr, "ERROR EfffectManager::addEffect: effect named '%s' have not implemented\n", type.c_str());
			return;
		}
		m_effectMap.insert(std::make_pair(type, pEffect));
		m_toFinalizeEffectVec.push_back(pEffect);
	}
}

Effect *EffectManager::getEffec(const std::string &type)
{
	EffectIter it = m_effectMap.find(type);
	if(it == m_effectMap.end())
	{
		fprintf(stderr, "ERROR EffectManager::getEffect: effect named '%s' not exists\n", type.c_str());
		return NULL;
	}
	else
		return it->second;
}

/// used by rendering thread
bool EffectManager::_finalize()
{
	for(ToFinalizeEffectIter it = m_toFinalizeEffectVec.begin();
			it != m_toFinalizeEffectVec.end(); ++it)
	{
		if((*it)->init() == false)
		{
			fprintf(stderr, "ERROR EffectManager::_finalize: effect compiled error\n");
			return false;
		}
	}
	m_toFinalizeEffectVec.clear();
	return true;
}
}
