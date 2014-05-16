#include "stdafx.h"
#include "Hurt.h"

CHurt::CHurt()
    : m_fHurtData(0.f)
{

}

CHurt::~CHurt()
{

}

void CHurt::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fHurtData, true, 0xFFFFFFFF, _T("ÉËº¦ÊıÖµ"), NULL, NULL, NULL);
}

void CHurt::Execute(SAIPackage& package)
{
    package.pTargetObj->Hurt(m_fHurtData);
}
