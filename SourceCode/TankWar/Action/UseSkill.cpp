#include "stdafx.h"
#include "UseSkill.h"
#include "../Skill/Skill.h"

CUseSkill::CUseSkill()
{

}

CUseSkill::~CUseSkill()
{

}

void CUseSkill::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);

    DECLARE_DEPENDENCY_LIST(serializer, m_skillList, _T("�����б�"), eDT_Strong);
}

void CUseSkill::Execute(SAIPackage& package)
{

}
