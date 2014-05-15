
#ifndef __CAMERAMANAGER_H_INCLUDED__
#define __CAMERAMANAGER_H_INCLUDED__

#include "PublicDef.h"
#include "Utility/core/Vector2d.h"
#include "Utility/core/Vector3d.h"
#include "Scene/CCameraSceneNode.h"


class CameraManager
{
	BEATS_DECLARE_SINGLETON(CameraManager);
public:


	void				Init();
	void				Update(int dt);

	void				OnKeyEvent();
	void				OnMouseEvent();
	core::vector3df		GetWorldUp() const;
	f32					GetTime();

public:
	scene::CCameraSceneNode* m_CameraNode;
	s32 m_keyDown[GLFW_KEY_Z];
	bool m_mouseLeftPress;
	core::vector2di			 m_mouseDelta;
	f32						 m_Time;			
};

#define S_CameraManager (CameraManager::GetInstance())

#endif
