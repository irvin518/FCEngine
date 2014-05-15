#include "stdafx.h"
#include "CAnimationManager.h"
#include "CAnimationImporter.h"
#include "CAnimationController.h"
#include "CArmatureData.h"
#include "CTextureData.h"
#include "CAnimationModel.h"

using namespace _2DSkeletalAnimation;

CAnimationManager *CAnimationManager::m_pInstance = nullptr;

CAnimationManager::CAnimationManager()
{

}

CAnimationManager::~CAnimationManager()
{
    for (auto  item : m_animationDataMap)
    {
        BEATS_SAFE_DELETE(item.second);
    }

    for (auto  item : m_armatureDataMap)
    {
        BEATS_SAFE_DELETE(item.second);
    }

    for (auto &item : m_textureList)
    {
        BEATS_SAFE_DELETE(item);
    }
}

CAnimationData* CAnimationManager::GetAnimationData(const std::string& name) const
{
    CAnimationData* pAnimationData = nullptr;
    std::map<std::string, CAnimationData*>::const_iterator iter = m_animationDataMap.find(name);
    BEATS_ASSERT(iter != m_animationDataMap.end());
    if(iter != m_animationDataMap.end())
    {
        pAnimationData = iter->second;
    }
    return pAnimationData;
}

CArmatureData* CAnimationManager::GetArmatureData(const std::string& name) const
{
    CArmatureData* pArmatureData = nullptr;
    std::map<std::string, CArmatureData*>::const_iterator iter = m_armatureDataMap.find(name);
    BEATS_ASSERT(iter != m_armatureDataMap.end());
    if(iter != m_armatureDataMap.end())
    {
        pArmatureData = iter->second;
    }
    return pArmatureData;
}

void CAnimationManager::AddAnimationData(CAnimationData* pAnimationData)
{
    BEATS_ASSERT(pAnimationData);
    m_animationDataMap[pAnimationData->GetName()] = pAnimationData;
    m_strLastAnimationName = pAnimationData->GetName();
}

void CAnimationManager::AddArmatureData(CArmatureData* pArmatureData)
{
    BEATS_ASSERT(pArmatureData);
    m_armatureDataMap[pArmatureData->GetName()] = pArmatureData;
    m_strLastArmatureName = pArmatureData->GetName();
}

std::vector<CTextureData*>& CAnimationManager::GetTextureData()
{
    return m_textureList;
}

const std::string CAnimationManager::GetLastAnimationName() const
{
    return m_strLastAnimationName;
}

const std::string CAnimationManager::GetLastArmatureName() const
{
    return m_strLastArmatureName;
}

void CAnimationManager::AddAnimationDataFromFile(const TString &file)
{
    CAnimationImporter::GetInstance()->ImportData(file);
}

CAnimationController* CAnimationManager::CreateController(CAnimationModel* pModel)
{
    BEATS_ASSERT(pModel);
    CAnimationController* aniController = new CAnimationController(pModel);
    m_controllerList.push_back(aniController);

    return aniController;
}

void CAnimationManager::DeleteController(CAnimationController *controller )
{
    auto itr = find(m_controllerList.begin(), m_controllerList.end(), controller);
    if(itr != m_controllerList.end())
    {
        BEATS_SAFE_DELETE(controller);
        m_controllerList.erase(itr);
    }
}

void CAnimationManager::Update(float fDeltaTime)
{
    for (auto pController : m_controllerList)
    {
        pController->Update(fDeltaTime);
    }
}

