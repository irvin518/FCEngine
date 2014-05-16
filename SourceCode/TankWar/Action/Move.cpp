#include "stdafx.h"
#include "Move.h"
#include "../TankComponent/Pedestal.h"
CMove::CMove()
{

}

CMove::~CMove()
{

}

void CMove::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_direction, true, 0xFFFFFFFF, _T("�ƶ�����"), NULL, NULL, NULL);
}

void CMove::Execute(SAIPackage& package)
{
    package.pSourceObj->GetPedesta()->Move(m_direction);
}

