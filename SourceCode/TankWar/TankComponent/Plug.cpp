#include "stdafx.h"
#include "Plug.h"

CPlug::CPlug()
{

}

CPlug::~CPlug()
{

}

void CPlug::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_plugList, _T("外挂装备列表"), eDT_Weak);
}

void CPlug::Updata()
{

}

CPlugConfigs::CPlugConfigs()
{

}

CPlugConfigs::~CPlugConfigs()
{
    //TODO?
    m_vPlugList.clear();
}

void CPlugConfigs::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_DEPENDENCY_LIST(serializer, m_vPlugList, _T("挂件配置"), eDT_Weak);
}
