
#include "StdAfx.h"
#include "CameraManager.h"
#include "Utility/core/math.h"
#include "Render/renderManager.h"

#define CAMERA_NEAR_VALUE	1
#define CAMERA_FAR_VALUE	2000

#define TRANS_VELOCITY  (2.f)
#define ROT_VELOCITY  (0.5f)

//---------------------------CameraManager---------------------------------------
CameraManager* CameraManager::m_pInstance = NULL;

CameraManager::CameraManager()
: m_CameraNode(NULL)
, m_mouseLeftPress(false)
{
	
	for (int i = 0; i < GLFW_KEY_Z; i++)
	{
		m_keyDown[i] = -1;
	}

}

CameraManager::~CameraManager()
{
	if ( m_CameraNode )
	{
		m_CameraNode->remove();
	}
}


void CameraManager::Init()
{	
	m_CameraNode = NEW scene::CCameraSceneNode();
	m_CameraNode->setNearValue( CAMERA_NEAR_VALUE );
	m_CameraNode->setFarValue( CAMERA_FAR_VALUE );
	m_CameraNode->setAspectRatio(1136.f/640);
	m_CameraNode->setUpVector(core::vector3df(0.0, 0.0, 1.0));

	core::vector3df postion = core::vector3df(0.0f, -10.0f, 0.f);
	m_CameraNode->setUpVector(core::vector3df(0.f, 0.f , 1.f));
	m_CameraNode->setPosition(postion);
	m_CameraNode->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));
	m_CameraNode->updateAbsolutePosition();
	m_CameraNode->recalculateMatrices();

	S_RenderManager->SetActiveCamera(m_CameraNode);
}

void CameraManager::Update(int dt)
{	
	OnKeyEvent();
	OnMouseEvent();
	m_Time += dt;

	if (m_mouseLeftPress)
	{
		float yaw = -m_mouseDelta.getX() * (dt / 1000.0f) * ROT_VELOCITY;
		float pitch = m_mouseDelta.getY() * (dt / 1000.0f) * ROT_VELOCITY;

		core::vector3df forward = m_CameraNode->getTarget() - m_CameraNode->getPosition();
		core::vector3df left = m_CameraNode->getUpVector().crossProduct(forward);
		left.normalize();

		forward.normalize();
		float angle = core::acos_(forward.dotProduct(m_CameraNode->getUpVector()));

		static const float ANGLE_EPSILON = 0.01f;
		if (angle + pitch < ANGLE_EPSILON)
			pitch = ANGLE_EPSILON - angle;
		else if (angle + pitch > core::PI - ANGLE_EPSILON)
			pitch = core::PI - ANGLE_EPSILON - angle;

		core::quaternion qYaw, qPitch;
		qYaw.fromAngleAxis(yaw, m_CameraNode->getUpVector());
		qPitch.fromAngleAxis(pitch, left);
		core::quaternion transformation = qYaw * qPitch;
		m_CameraNode->setTarget(transformation * forward + m_CameraNode->getPosition());

		m_mouseDelta.set(0, 0);   // should clear since we don't want to keep rotate...
	}

	// translate
	{
		core::vector3df forward = m_CameraNode->getTarget() - m_CameraNode->getPosition();
		forward.normalize();
		core::vector3df right = forward.crossProduct(m_CameraNode->getUpVector());
		right.normalize();
		core::vector3df up = right.crossProduct(forward);

		core::vector3df dir(0, 0, 0);

		if (m_keyDown[GLFW_KEY_W] > 0)	dir += forward;
		if (m_keyDown[GLFW_KEY_S] > 0)	dir -= forward;
		if (m_keyDown[GLFW_KEY_Q] > 0)	dir += up;
		if (m_keyDown[GLFW_KEY_E] > 0)	dir -= up;
		if (m_keyDown[GLFW_KEY_D] > 0)	dir += right;
		if (m_keyDown[GLFW_KEY_A] > 0)	dir -= right;

		dir.normalize();
		core::vector3df t = dir * (dt / 1000.0F) * TRANS_VELOCITY;
		if (t != core::vector3df())
		{
			m_CameraNode->setPosition(m_CameraNode->getPosition() + t);
			m_CameraNode->setTarget(m_CameraNode->getTarget() + t);


			INFO_OUT("camera position x =%f y = %f z = %f",m_CameraNode->getPosition().x(),m_CameraNode->getPosition().y(),m_CameraNode->getPosition().z());
		}
		
		m_CameraNode->updateAbsolutePosition();
		m_CameraNode->recalculateMatrices();
	}
}

f32 CameraManager::GetTime()
{
	return m_Time;
}


void CameraManager::OnKeyEvent()
{

	m_keyDown[GLFW_KEY_A]  = glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_A);
	m_keyDown[GLFW_KEY_D]  = glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_D);
	m_keyDown[GLFW_KEY_W]  = glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_W);
	m_keyDown[GLFW_KEY_S]  =  glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_S);
	m_keyDown[GLFW_KEY_Q]  = glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_Q);
	m_keyDown[GLFW_KEY_E]  = glfwGetKey(CRenderManager::GetInstance()->GetMainWindow(),GLFW_KEY_E) ;
}

void CameraManager::OnMouseEvent()
{
	static core::vector2di prevMousePosition;
	if (S_RenderManager->IsLeftMouseDown())
	{
		if(!m_mouseLeftPress)  
		{
			u32 x, y;
			S_RenderManager->GetMousePos(x, y);
			prevMousePosition.set(x, y);
			m_mouseDelta.set(0, 0);
			m_mouseLeftPress = true;
		}
	}
	else
	{
		m_mouseLeftPress = false;
	}

	
	if (m_mouseLeftPress)
	{
		u32 x, y;
		S_RenderManager->GetMousePos(x, y);
		m_mouseDelta = core::vector2di(x, y) - prevMousePosition;
		prevMousePosition.set(x, y);
	}
	
}

core::vector3df CameraManager::GetWorldUp() const
{
	return core::vector3df(0, 0, 1);
}