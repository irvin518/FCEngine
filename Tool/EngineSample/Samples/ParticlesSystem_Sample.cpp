////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "ParticlesSystem_Sample.h"

#include "CameraManager.h"
#include "ParticlesSystemManager.h"

#include "Render/RenderManager.h"
#include "Render/Material/CMaterial.h"
#include "Render/Material/CMaterialFactory.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

ParticlesSystem_Sample::ParticlesSystem_Sample()
{
	//int w,h;
	//S_Application->GetScreenSize(w, h);
	//ASpritePtr m_Sprite = GetSpriteManager()->Sprite_Get("particles_effect",TextureRequirement_LoadTextureSync);
	//m_Sprite->SetShader(k_Shader_particles, m_Sprite->m_uniforms);
	//m_Player = NEW GLLibPlayer(m_Sprite, 1136/2, 640/2);
	//m_Player->SetAnim(0, -1);
	
	//CMaterial* m_Material = CMaterialFactory::CreateMaterial("MATERIALPARTICLES");
	//m_Material->SetBaseTechnique("add");

	
	S_CameraManager->Init();


	GetPSManger()->Load("effects.bin");

	m_effectPosition = NEW scene::ISceneNode();
	m_effectPosition->setPosition(0.f, -3.f, 0.f);
	m_effectPosition->updateAbsolutePosition();
	GetPSManger()->SP_Play("mc_male_book", m_effectPosition);
	GetPSManger()->SP_Play("battle_trans", m_effectPosition);

	m_sceneSparkPosition = NEW scene::ISceneNode();
	m_sceneSparkPosition->setPosition(15.f, 15.f, 5.f);
	m_sceneSparkPosition->setScale(core::vector3df(5.0f, 5.0f, 5.0f));
	m_sceneSparkPosition->updateAbsolutePosition();
	GetPSManger()->SP_Play("scene_spark",m_sceneSparkPosition);
	//GetPSManger()->SP_Play("mc_skill_heal",m_effectPosition);

	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
ParticlesSystem_Sample::~ParticlesSystem_Sample()
{
  
}


void ParticlesSystem_Sample::Pause()
{
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
void ParticlesSystem_Sample::Resume()
{
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
void ParticlesSystem_Sample::Release()
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////////////////////////////////////////////////////////////////
void ParticlesSystem_Sample::Update(int dt)
{
	S_CameraManager->Update(dt);
	GetPSManger()->SP_Update((f32)dt);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void ParticlesSystem_Sample::Render()
{

	GetPSManger()->SP_Render();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
