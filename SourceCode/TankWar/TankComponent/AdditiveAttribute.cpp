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
    DECLARE_PROPERTY( serializer, m_fAttack, true, 0xFFFFFFFF, _T( "���ӹ���" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fDefense, true, 0xFFFFFFFF, _T( "���ӷ���" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fHP, true, 0xFFFFFFFF, _T( "��������" ), NULL, NULL, NULL);
    DECLARE_PROPERTY( serializer, m_fRecSpeed, true, 0xFFFFFFFF, _T( "��׼�ظ��ٶ�" ), NULL, NULL, NULL);    
}
