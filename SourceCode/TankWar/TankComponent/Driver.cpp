#include "stdafx.h"
#include "Driver.h"
#include "TankWar/Skill/Skill.h"

CDriver::CDriver()
    : m_pAttribute(nullptr)
{

}

CDriver::~CDriver()
{
    //TODO？
    m_vSkillList.clear();
}

void CDriver::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vSkillList, _T("附带技能"), eDT_Weak);
    DECLARE_PROPERTY( serializer, m_pAttribute, true, 0xFFFFFFFF, _T( "附加属性" ), NULL, NULL, NULL);
}

CDriverConfigs::CDriverConfigs()
{

}

CDriverConfigs::~CDriverConfigs()
{
    //TODO？
    m_vDriverList.clear();
}

void CDriverConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vDriverList, _T("驾驶员配置"), eDT_Weak);
}
