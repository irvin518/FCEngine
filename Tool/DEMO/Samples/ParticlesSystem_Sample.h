#ifndef __PARTICLES_SYSTEMS_H__
#define __PARTICLES_SYSTEMS_H__

#include "SampleStates.h"

#include "scene/ISceneNode.h"

class ParticlesSystem_Sample: public gxSampleState
{
public:

	ParticlesSystem_Sample();

	~ParticlesSystem_Sample();

 
	virtual void Update(int dt);

	virtual void Pause();

	virtual void Render();

	virtual void Release();

	virtual void Resume();

private:

	scene::ISceneNode* m_effectPosition;
	scene::ISceneNode* m_sceneSparkPosition;
};

#endif //__PARTICLES_SYSTEMS_H__