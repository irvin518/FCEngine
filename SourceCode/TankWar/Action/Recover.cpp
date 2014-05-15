#include "stdafx.h"
#include "Recover.h"

CRecover::CRecover()
{

}

CRecover::~CRecover()
{

}

void CRecover::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fRecoverHP,true, 0xFFFFFFFF, _T("»Ø¸´ÉúÃüÖµ"), NULL, NULL, NULL);
}

void CRecover::Execute(CTank& tank)
{
    tank.Recover(m_fRecoverHP);
}
