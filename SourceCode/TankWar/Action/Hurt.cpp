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
    DECLARE_PROPERTY(serializer, m_fHurtData, true, 0xFFFFFFFF, _T("ÉËº¦ÊýÖµ"), NULL, NULL, NULL);
}

void CHurt::Execute(CTank& tank)
{
    tank.Hurt(m_fHurtData);
}
