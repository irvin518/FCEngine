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
    DECLARE_DEPENDENCY(serializer, m_pNextAction, _T("下一步动作"), eDT_Weak);
}

CActionBase* CActionBase::GetNextAction()
{
    return m_pNextAction;
}
