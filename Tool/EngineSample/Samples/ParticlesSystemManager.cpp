#include "stdafx.h"
#include "ParticlesSystemManager.h"


namespace
{
	int GetID()
	{
		static int id = 0;
		return id++; 
	}
}

ParticlesSystemManager* ParticlesSystemManager::m_pInstance = NULL;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ParticlesSystemManager::ParticlesSystemManager()
{
	Clean();

}
ParticlesSystemManager::~ParticlesSystemManager()
{
	Clean();
}
int ParticlesSystemManager::Load(const char* filename)
{
	// release previous one
	Clean();

	// load
	int code = S_EffectsManager->Deserialize(filename);
	if (0 == code)
	{
		S_EffectsManager->GetEffectStream()->BeginRead();
		for (int i = 0; i < S_EffectsManager->GetEffectsCount(); i++)
		{
			CPSEffectPtr pEff(new CPSEffect());
			pEff->SetParent(NULL);
			pEff->Deserialize(*(S_EffectsManager->GetEffectStream()));
			pEff->SetVisible(false);
			m_effectTemplate[std::string(pEff->GetEffectName())] = pEff;
		}
		S_EffectsManager->GetEffectStream()->EndRead();
	}

	return code;
}

int ParticlesSystemManager::SP_Play(std::string psName, scene::ISceneNode* node)
{
	// get param from CParamSet
	std::string name = psName;
	// new instance
	CPSEffectPtr eff;
	eff = Clone(name.c_str());
	eff->SetEffectName(name.c_str());
	eff->SetParent(node);
	eff->SetScaleParticles(true);

	int id = GetID();
	m_effRuntimeInst[id] = eff;
	return id;
}


void ParticlesSystemManager::SP_Render()
{
	for (SEffectRuntimeInst::iterator it = m_effRuntimeInst.begin();
			it != m_effRuntimeInst.end(); it++)
	{
		it->second->Render();
	}
}

void ParticlesSystemManager::SP_Update(f32 dt)
{
	for (SEffectRuntimeInst::iterator it = m_effRuntimeInst.begin();
			it != m_effRuntimeInst.end(); it++)
	{
		it->second->Update(dt);
	}
}


CPSEffectPtr ParticlesSystemManager::Clone(const char* name)
{
	// find effect
	stEffectTemplate::iterator it = m_effectTemplate.find(name);
	if (it == m_effectTemplate.end())
		return CPSEffectPtr();

	// clone a new one
	CPSEffectPtr newEff(it->second->Clone());
	newEff->SetVisible(true);
	return newEff;
}
void ParticlesSystemManager::Clean()
{
	m_effectTemplate.clear();
}

