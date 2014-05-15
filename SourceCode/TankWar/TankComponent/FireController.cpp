#include "stdafx.h"
#include "FireController.h"

CFireController::CFireController()
    : m_fSectorRadius(100.f)
    , m_fSectorAngle(45.f)
    , m_fMaxAnglePercent(0.8f)
    , m_fMinAngelPercent(0.5f)
    , m_fShrinkSpeed(1.f)
{

}

CFireController::~CFireController()
{

}

void CFireController::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fSectorRadius, true, 0xFFFFFFFF, _T("外扇形半径"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fSectorAngle, true, 0xFFFFFFFF, _T("外扇形角度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fMaxAnglePercent, true, 0xFFFFFFFF, _T("内扇形最大角度比"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fMinAngelPercent, true, 0xFFFFFFFF, _T("内扇形最小角度比"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fShrinkSpeed, true, 0xFFFFFFFF, _T("内扇形收缩速度"), NULL, NULL, NULL);
}


CFireControllerConfigs::CFireControllerConfigs()
{

}

CFireControllerConfigs::~CFireControllerConfigs()
{
    //TODO?
    m_vFireControllerList.clear();
}

void CFireControllerConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vFireControllerList, _T("火控系统配置"), eDT_Weak);
}
