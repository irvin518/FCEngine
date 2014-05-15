#include "stdafx.h"
#include "AdditiveAttribute.h"

CAdditiveAttribute::CAdditiveAttribute()
{

}

CAdditiveAttribute::~CAdditiveAttribute()
{

}

void CAdditiveAttribute::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY( serializer, m_fAttack, true, 0xFFFFFFFF, _T( "附加攻击" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fDefense, true, 0xFFFFFFFF, _T( "附加防御" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fHP, true, 0xFFFFFFFF, _T( "附加生命" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fRecSpeed, true, 0xFFFFFFFF, _T( "瞄准回复速度" ), NULL, NULL, NULL);    
}
