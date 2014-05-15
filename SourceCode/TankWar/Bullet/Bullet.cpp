#include "stdafx.h"
#include "Bullet.h"

CBullet::CBullet()
{
    m_bIsTrace = false;
    m_fSpeed = 1.0f;
    m_fMaxSpeed = 1.0f;
    m_fMaxDistance = 1.0f;
    m_nBulletSkillID = 0;
    m_nDamageFloat = 1;
    m_nDamageScope = 1;
    m_uDamage = 1;
    m_uSteeringAngularVelocity = 30;
    m_fAcceleration = 2.0f;
}

CBullet::~CBullet()
{

}

void CBullet::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY( serializer, m_bIsTrace, true, 0xFFFFFFFF, _T( "是否跟踪" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_fSpeed, true, 0xFFFFFFFF, _T( "速度" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_fMaxSpeed, true, 0xFFFFFFFF, _T( "最大速度" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_fMaxDistance, true, 0xFFFFFFFF, _T( "最大距离" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_fAcceleration, true, 0xFFFFFFFF, _T( "加速度" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_nBulletSkillID, true, 0xFFFFFFFF, _T( "附带技能" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_nDamageFloat, true, 0xFFFFFFFF, _T( "伤害浮动范围" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_nDamageScope, true, 0xFFFFFFFF, _T( "伤害范围" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_uDamage, true, 0xFFFFFFFF, _T( "伤害值" ), NULL, NULL, NULL );
    DECLARE_PROPERTY( serializer, m_uSteeringAngularVelocity, true, 0xFFFFFFFF, _T( "转向角速度" ), NULL, NULL, NULL );
}

CBulletConfigs::CBulletConfigs()
{

}

CBulletConfigs::~CBulletConfigs()
{
    //TODO?
    m_vBulletList.clear();
}

void CBulletConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vBulletList, _T("子弹配置"), eDT_Weak);
}
