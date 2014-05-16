#ifndef RESOURCE_RESOURCEMANAGER_H__INCLUDE
#define RESOURCE_RESOURCEMANAGER_H__INCLUDE

#include <Queue>
#include <mutex>
#include <condition_variable>
#include "Resource.h"
#include "Utility/BeatsUtility/SharePtr.h"
#include "ResourcePathManager.h"

class CResourceManager
{
    BEATS_DECLARE_SINGLETON(CResourceManager);
public:
    bool InitLoadThread();
    template <typename T>
    SharePtr<T> GetResource(const TString &filename, bool bAsync);
    bool QueryResource(const TString& fileName, SharePtr<CResource>& output) const;
    bool CleanUp();
    std::condition_variable& GetLoadCondition();
    std::mutex& GetLoadMutex();

    // Don't use it out of CResource.
    void RegisterResource(const SharePtr<CResource>& pResource);
    void UnregisterResource(const SharePtr<CResource>& pResource);
    bool LoadResource(SharePtr<CResource> pResource, bool bAsync);
    TString GetFullPath(const TString &strFileName, EResourceType type) const;

private:
    bool CreateLoadThread();
    static void LoadResourceThreadFunc(CResourceManager* pMgr);

private:
    bool m_bQuit;
    bool m_bInitialized;
    typedef std::queue<SharePtr<CResource>> TResourceLoadQueue;
    typedef std::map<TString, SharePtr<CResource>> TResourceNameMap;
    TResourceLoadQueue m_loadQueue;
    TResourceNameMap m_resource;
    std::thread m_loadThread;
    std::mutex m_loadQueueMutex;

    std::condition_variable m_loadCondition;
    std::mutex m_loadMutex;

};

template <typename T>
SharePtr<T> CResourceManager::GetResource( const TString &strFilePath, bool bAsync )
{
    SharePtr<T> pRet;
    TString strFullPath = GetFullPath(strFilePath, T::RESOURCE_TYPE);
    TResourceNameMap::iterator iter = m_resource.find(strFullPath);
    if(iter == m_resource.end())
    {
        pRet = new T;
        pRet->SetFilePath(strFullPath);
        LoadResource(pRet, bAsync);
    }
    else
    {
        pRet = iter->second;
    }
    return pRet;
}

#endif