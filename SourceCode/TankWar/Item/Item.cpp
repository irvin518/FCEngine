#include "stdafx.h"
#include "Item.h"

CItem::CItem()
{

}

CItem::~CItem()
{

}

void CItem::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
}

CItemConfigs::CItemConfigs()
{

}

CItemConfigs::~CItemConfigs()
{
    //TODO?
    m_vItemList.clear();
}

void CItemConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vItemList, _T("ŒÔ∆∑≈‰÷√"), eDT_Weak);
}
