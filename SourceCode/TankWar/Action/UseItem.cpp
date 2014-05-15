#include "stdafx.h"
#include "UseItem.h"
#include "../Item/Item.h"
#include "../TankComponent/Tank.h"

CUseItem::CUseItem()
{

}

CUseItem::~CUseItem()
{

}

void CUseItem::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY(serializer, m_pItem, _T("Îï¼þ"), eDT_Strong);
}


void CUseItem::Execute(CTank& tank)
{

}

