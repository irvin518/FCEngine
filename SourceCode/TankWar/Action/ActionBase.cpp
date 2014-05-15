#include "stdafx.h"
#include "ActionBase.h"

CActionBase::CActionBase()
{

}

CActionBase::~CActionBase()
{

}

void CActionBase::ReflectData(CSerializer& serializer)
{
    DECLARE_DEPENDENCY(serializer, m_pNextAction, _T("��һ������"), eDT_Weak);
}

CActionBase* CActionBase::GetNextAction()
{
    return m_pNextAction;
}
