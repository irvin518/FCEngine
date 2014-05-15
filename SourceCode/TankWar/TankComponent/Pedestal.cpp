#include "stdafx.h"
#include "Pedestal.h"

CPedesta::CPedesta()
    : m_bMove(false)
    , m_fMaxMoveSpeed(5.f)
    , m_fFaccelerated(1.f)
    , m_fAngularSpeed(10.f)
{
    kmVec2Fill(&m_currDirection,0.f, 1.f);
}

CPedesta::~CPedesta()
{

}

void  CPedesta::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fMaxMoveSpeed, true, 0xFFFFFF, _T("����ƶ��ٶ�"),NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fFaccelerated, true, 0xFFFFFF, _T("ǰ�����ٶ�"),NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fAngularSpeed, true, 0xFFFFFF, _T("ת����ٶ�"),NULL, NULL, NULL);
}

void CPedesta::Update(float fDeltaTime)
{
    if(m_bMove)
    {
        //calc angle and postion

    }
}

void CPedesta::Move(kmVec2 direction)
{
    m_bMove = true;
    m_targeyDirection.x = direction.x;
    m_targeyDirection.y = direction.y;
}

void CPedesta::Stop()
{
    m_bMove = false;
}


CPedestaConfigs::CPedestaConfigs()
{

}

CPedestaConfigs::~CPedestaConfigs()
{
    //TODO?
    m_vPedestaList.clear();
}

void CPedestaConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vPedestaList, _T("��������"), eDT_Weak);
}
