#include "stdafx.h"
#include "MainWeapon.h"
CMainWeapon::CMainWeapon()
{

}

CMainWeapon::~CMainWeapon()
{

}

void CMainWeapon::Update(float fDeltaTime)
{

}

void CMainWeapon::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY(serializer, m_pSkill, _T("¼¼ÄÜ"), eDT_Strong);
}

void CMainWeapon::Shot()
{

}


CMainWeaponConfigs::CMainWeaponConfigs()
{

}

CMainWeaponConfigs::~CMainWeaponConfigs()
{
    //TODO?
    m_vMainWeaponList.clear();
}

void CMainWeaponConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vMainWeaponList, _T("Ö÷ÎäÆ÷ÅäÖÃ"), eDT_Weak);
}
