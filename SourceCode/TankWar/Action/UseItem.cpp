#include "stdafx.h"
#include "UseItem.h"

CUseItem::CUseItem()
    : m_iItemID(0)
{

}

CUseItem::~CUseItem()
{

}

void CUseItem::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_iItemID, true, 0xFFFFFFFF, _T("���ID"), NULL, NULL, NULL);
}


void CUseItem::Execute(SAIPackage& package)
{
    package.pSourceObj->UseItem(m_iItemID);
}

