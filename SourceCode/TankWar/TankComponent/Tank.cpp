#include "stdafx.h"
#include "Tank.h"
#include "Pedestal.h"
#include "Turret.h"
#include "FireController.h"
#include "Shield.h"
CTank::CTank()
    : m_fHP(100.f)
    , m_pPedesta(nullptr)
    , m_pTurret(nullptr)
    , m_pFireController(nullptr)
    , m_pShield(nullptr)
    , m_eTankState(eTS_TANKSTATE_NORMAL)
{
}

CTank::~CTank()
{
    BEATS_SAFE_DELETE(m_pPedesta);
    BEATS_SAFE_DELETE(m_pTurret);
    BEATS_SAFE_DELETE(m_pFireController);
    BEATS_SAFE_DELETE(m_pShield);
}

void CTank::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);

    DECLARE_DEPENDENCY(serializer, m_pPedesta, _T("µ◊◊˘"),eDT_Strong);
    DECLARE_DEPENDENCY(serializer, m_pTurret, _T("≈⁄À˛"),eDT_Strong);
    DECLARE_DEPENDENCY(serializer, m_pFireController, _T("ªøÿœµÕ≥"),eDT_Strong);
    DECLARE_DEPENDENCY(serializer, m_pShield, _T("ª§∂‹"),eDT_Weak);
}

void CTank::Update(float fDeltaTime)
{
    m_pPedesta->Update(fDeltaTime);
    m_pTurret->Update(fDeltaTime);
    m_pFireController->Update(fDeltaTime);
    m_pShield->Update(fDeltaTime);
}

CTurret* CTank::GetTurret() const
{
    return m_pTurret;
}

CPedesta* CTank::GetPedesta() const
{
    return m_pPedesta;
}

void CTank::Hurt(kmScalar fHurtData)
{
    kmScalar fRetHurtData = m_pShield->Hurt(fHurtData);

    m_fHP -= fRetHurtData;
}

void CTank::Die()
{
    m_eTankState = eTS_TANKSTATE_DIE;
}

bool CTank::IsDie()
{
    return m_eTankState == eTS_TANKSTATE_DIE;
}

void CTank::Recover(kmScalar fRecoverHP)
{
    m_fHP += fRecoverHP;
}

CTankConfigs::CTankConfigs()
{

}

CTankConfigs::~CTankConfigs()
{
    //TODO?
    m_vTankList.clear();
}

void CTankConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vTankList, _T("ÃπøÀ≈‰÷√"), eDT_Weak);
}
