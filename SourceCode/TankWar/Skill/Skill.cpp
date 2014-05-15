#include "stdafx.h"
#include "Skill.h"

CSkill::CSkill()
{
    m_pBullet = nullptr;
    m_bWeapon = false;
}

CSkill::~CSkill()
{    
}

void CSkill::ReflectData(CSerializer& serializer)
{
     super::ReflectData(serializer);
     DECLARE_PROPERTY( serializer, m_bWeapon, true, 0xFFFFFFFF, _T( "是否发射子弹" ), NULL, NULL, NULL );
     DECLARE_PROPERTY( serializer, m_pBullet, true, 0xFFFFFFFF, _T( "子弹" ), NULL, NULL, _T("VisibleWhen:m_bWeapon == true"));
}

CSkillConfigs::CSkillConfigs()
{

}

CSkillConfigs::~CSkillConfigs()
{
    //TODO?
    m_vSkillList.clear();
}

void CSkillConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vSkillList, _T("技能配置"), eDT_Weak);
}
