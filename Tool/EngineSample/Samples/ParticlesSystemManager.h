#ifndef EXT_PSMGR_H_
#define EXT_PSMGR_H_

#include <list>
#include <map>

#include <ParticlesSystem/EffectsManager.h>
#include <ParticlesSystem/CPSEffect.h>

#include <Scene/ISceneNode.h>

class ParticlesSystemManager
{

	BEATS_DECLARE_SINGLETON(ParticlesSystemManager);
public:


	int Load(const char* filename);
	CPSEffectPtr Clone(const char* name);
	void Clean();
	
	int SP_Play(std::string psName, scene::ISceneNode* node);
	void SP_Update(f32 ms);
	void SP_Render();
private:
	typedef std::map<std::string, CPSEffectPtr> stEffectTemplate;
	stEffectTemplate m_effectTemplate;

	typedef std::map<int, CPSEffectPtr> SEffectRuntimeInst;
	SEffectRuntimeInst m_effRuntimeInst;
};

#define GetPSManger() (ParticlesSystemManager::GetInstance())

#endif
