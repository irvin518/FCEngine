#include "stdafx.h"
#include "Turret.h"
#include "MainWeapon.h"
#include "SubWeapon.h"
#include <mmsystem.h>
CTurret::CTurret()
    : m_eActiveWeaponType(eWT_MAINWEAPON)
    , m_fFireInterval(1.f)
    , m_uSingleBulletCount(50)
    , m_uLoaddingSpeed(20)
    , m_fAngleSpeed(20.f)
{

}

CTurret::~CTurret()
{

}

void CTurret::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY( serializer, m_fFireInterval, true, 0xFFFFFFFF, _T( "射击间隔" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_uSingleBulletCount, true, 0xFFFFFFFF, _T( "单组载弹量" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_uLoaddingSpeed, true, 0xFFFFFFFF, _T( "填装速度" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_fAngleSpeed, true, 0xFFFFFFFF, _T("转向角速度"), NULL, NULL, NULL);
    DECLARE_DEPENDENCY( serializer, m_pMainWeapon, _T("主武器"), eDT_Weak);
    DECLARE_DEPENDENCY( serializer, m_pSubWeapon, _T("副武器"), eDT_Weak);
}

bool CTurret::IsCanShot()
{
    bool bRet = false;
    unsigned long curr = timeGetTime();
    unsigned long delta = curr - m_lLastShotTime;
    if(delta >= m_fFireInterval)
        bRet = true;
    return bRet;
}

void CTurret::Update(float fDeltaTime)
{
    m_pMainWeapon->Update(fDeltaTime);
    m_pSubWeapon->Update(fDeltaTime);
}

void CTurret::Shot()
{
    if(IsCanShot())
    {
        m_lLastShotTime = timeGetTime();

        if(eWT_MAINWEAPON == m_eActiveWeaponType)
        {
            m_pMainWeapon->Shot();
        }
        else if(eWT_SUBWEAPON == m_eActiveWeaponType)
        {
            
        }
    }
}


CTurretConfigs::CTurretConfigs()
{

}

CTurretConfigs::~CTurretConfigs()
{
    //TODO?
    m_vTurretList.clear();
}

void CTurretConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vTurretList, _T("炮塔配置"), eDT_Weak);
}
