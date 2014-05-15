#include "stdafx.h"
#include "Shield.h"

CShield::CShield()
    : m_fHP(100.f)
    , m_fHPReSpeed(0.5f)
{

}

CShield::~CShield()
{

}

void CShield::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fHP, true, 0xFFFFFFFF, _T("HP"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fHPReSpeed, true, 0xFFFFFFFF, _T("HP回复速度"), NULL, NULL, NULL);
}

void CShield::Update(float fDeltaTime)
{

}

kmScalar CShield::GetHP() const
{
    return m_fHP;
}

void CShield::SetHP(const kmScalar fHP)
{
    m_fHP = fHP;
}

kmScalar CShield::Hurt(kmScalar fHurtData)
{
    kmScalar fRetHurtData = 0.f;
    if(m_fHP > fHurtData)
    {
        m_fHP -= fHurtData;
    }
    else
    {
        fRetHurtData = fHurtData - m_fHP;
    }

    return fRetHurtData;
}

CShieldConfigs::CShieldConfigs()
{

}

CShieldConfigs::~CShieldConfigs()
{
    //TODO?
    m_vShieldList.clear();
}

void CShieldConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vShieldList, _T("护盾配置"), eDT_Weak);
}
