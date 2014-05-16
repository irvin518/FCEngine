#include "stdafx.h"
#include "ResourceShlwapiManager.h"

CResourceShlwapiManager* CResourceShlwapiManager::m_pInstance = NULL;

CResourceShlwapiManager::CResourceShlwapiManager()
{

}
CResourceShlwapiManager::~CResourceShlwapiManager()
{

}

const TCHAR* CResourceShlwapiManager::PathFindExtension(const TCHAR* pszPath )
{
    assert (pszPath != NULL);
    TString file_name = pszPath;
    int pos = file_name.find(_T("."));
    int len = file_name.length();
    if (pos != -1)
    {
        m_strResultCache = file_name.substr(pos, len - pos);
    }
    else
    {
        m_strResultCache = _T("");
    }
    return m_strResultCache.c_str();
}

const TCHAR* CResourceShlwapiManager::PathFindFileName( const TCHAR* pszPath )
{
    assert (pszPath != NULL);
    TString file_name = pszPath;
    int len = file_name.length();
    int pos = file_name.rfind(_T("\\"), len);
    if (pos != -1)
    {
        m_strResultCache = file_name.substr(pos + 1, len - pos - 1);
    }
    else
    {
        m_strResultCache = _T("");
    }
    return m_strResultCache.c_str();
}

BOOL CResourceShlwapiManager::IsPathFileExists( const TCHAR* pszPath )
{
    FILE *pFile = _tfopen(pszPath, _T("rb"));
    return pFile != NULL ? TRUE : FALSE;
}
