#include "stdafx.h"
#include "SubWeapon.h"

CSubWeapon::CSubWeapon()
    : m_uBulletMaxCount(10)
{

}

CSubWeapon::~CSubWeapon()
{

}

void CSubWeapon::Update(float fDeltaTime)
{

}

void CSubWeapon::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_uBulletMaxCount, true,0xFFFFFFFF, _T("��������"), NULL, NULL, NULL);
}



CSubWeaponConfigs::CSubWeaponConfigs()
{

}

CSubWeaponConfigs::~CSubWeaponConfigs()
{
    //TODO?
    m_vSubWeaponList.clear();
}

void CSubWeaponConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vSubWeaponList, _T("����������"), eDT_Weak);
}
