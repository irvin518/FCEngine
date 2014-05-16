#include "stdafx.h"
#include "Resource.h"
#include "ResourceManager.h"

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

bool CResource::OnPropertyChange(void* pVariableAddr, void* pNewValueToBeSet)
{
    bool bRet = false;
    if (!super::OnPropertyChange(pVariableAddr, pNewValueToBeSet))
    {
        if (pVariableAddr == &m_strPath)
        {
            if (m_strPath.m_value.length() > 0) //change path
            {
                SharePtr<CResource> pRet;
                bool bResourceExists = CResourceManager::GetInstance()->QueryResource(m_strPath.m_value, pRet);
                if (bResourceExists)
                {
                    BEATS_ASSERT(pRet.Get() == this);
                    CResourceManager::GetInstance()->UnregisterResource(pRet);
                    pRet->Unload();
                }
                DeserializeVarialble(pRet->m_strPath, (CSerializer*)pNewValueToBeSet);
                if(pRet->m_strPath.m_value.length() > 0)
                {
                    pRet->SetFilePath(CResourceManager::GetInstance()->
                        GetFullPath(pRet->m_strPath.m_value, pRet->GetType()));
                    CResourceManager::GetInstance()->LoadResource(pRet, false);
                }
            }
            else    //first set path
            {
                DeserializeVarialble(m_strPath, (CSerializer*)pNewValueToBeSet);
                SetFilePath(CResourceManager::GetInstance()->GetFullPath(m_strPath.m_value, GetType()));
                CResourceManager::GetInstance()->LoadResource(this, false);
            }
            bRet = true;
        }
    }
    return bRet;
}

bool CResource::IsLoaded() const
{
    return m_bIsLoaded;
}
