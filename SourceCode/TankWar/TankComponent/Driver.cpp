#include "stdafx.h"
#include "Driver.h"
#include "TankWar/Skill/Skill.h"

CDriver::CDriver()
    : m_pAttribute(nullptr)
{

}

CDriver::~CDriver()
{
    //TODO��
    m_vSkillList.clear();
}

void CDriver::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vSkillList, _T("��������"), eDT_Weak);
    DECLARE_PROPERTY( serializer, m_pAttribute, true, 0xFFFFFFFF, _T( "��������" ), NULL, NULL, NULL);
}

CDriverConfigs::CDriverConfigs()
{

}

CDriverConfigs::~CDriverConfigs()
{
    //TODO��
    m_vDriverList.clear();
}

void CDriverConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vDriverList, _T("��ʻԱ����"), eDT_Weak);
}
