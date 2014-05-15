#include "stdafx.h"
#include "Resource.h"

CResource::CResource()
    : m_bIsLoaded(false)
{

}

CResource::~CResource()
{
}

void CResource::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_strPath, true, 0xFF0000FF, _T("ÎÄ¼þÂ·¾¶"), NULL, NULL, NULL);
}

const TString& CResource::GetFilePath() const
{
    return m_strPath.m_value;
}

void CResource::SetFilePath(const TString& str)
{
    m_strPath.m_value = str;
}

void CResource::SetLoadedFlag(bool bFlag)
{
    m_bIsLoaded = bFlag;
}

bool CResource::IsLoaded() const
{
    return m_bIsLoaded;
}
