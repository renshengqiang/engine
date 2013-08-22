#ifndef _ENGINE_EFFECT_MANAGER_H
#define _ENGINE_EFFECT_MANAGER_H
#include <UtilSingleton.h>
#include <string>
#include <map>
#include <vector>

namespace Engine
{
class Effect;
class EffectManager: public Util::Singleton<EffectManager>
{
public:
	typedef std::map<std::string, Effect*>	EffectMap;
	typedef EffectMap::iterator			EffectIter;
	typedef std::vector<Effect*>			ToFinalizeEffectVec;
	typedef ToFinalizeEffectVec::iterator	ToFinalizeEffectIter;
	
	EffectManager();
	~EffectManager();

	void addEffect(const std::string &type, const char *vsFile, const char *fsFile);
	Effect *getEffec(const std::string &type);

	/// used by rendering thread
	bool _finalize();
private:
	EffectMap				m_effectMap;
	ToFinalizeEffectVec 		m_toFinalizeEffectVec;
};
}

#endif
