#include "stdafx.h"
#include "CArmatureData.h"
#include "CBoneData.h"

using namespace _2DSkeletalAnimation;

CArmatureData::CArmatureData()
{

}

CArmatureData::~CArmatureData()
{
    for (auto &item : m_boneDataMap)
    {
        CBoneData* pBoneData = item.second;
        BEATS_SAFE_DELETE(pBoneData);
    }
}

void CArmatureData::AddBoneData(CBoneData *boneData)
{
    m_boneDataMap[boneData->GetName()] = boneData;

    size_t insertPos = 0;
    for (insertPos = 0; insertPos < m_boneDataOrderList.size(); ++insertPos)
    {
        if(boneData->GetZOrder() < m_boneDataOrderList[insertPos]->GetZOrder())
        {
            break;
        }
    }

    m_boneDataOrderList.insert(m_boneDataOrderList.begin() + insertPos, boneData);
}

const CBoneData* CArmatureData::GetBoneData(const std::string &boneName) const
{
    CBoneData* pRet = nullptr;
    BonesDataMap ::const_iterator iter = m_boneDataMap.find(boneName);
    BEATS_ASSERT(iter != m_boneDataMap.end());
    if (iter != m_boneDataMap.end())
    {
        pRet = iter->second;
    }
    return pRet;
}

const std::string& CArmatureData::GetName() const
{
    return m_strName;
}

void CArmatureData::SetName(const std::string& name)
{
    m_strName = name;
}

const CArmatureData::BonesDataMap& CArmatureData::GetBonesMap() const
{
    return m_boneDataMap;
}

const std::vector<CBoneData*>& CArmatureData::GetBoneOrderList()
{
    return m_boneDataOrderList;
}

SDisplayData CArmatureData::ReplaceDisplayData(const std::string boneName, const SDisplayData& displayData)
{
    BonesDataMap ::const_iterator iter = m_boneDataMap.find(boneName);
    BEATS_ASSERT(iter != m_boneDataMap.end());
    CBoneData* pBoneData = iter->second;
    return pBoneData->ReplaceDisplayData(displayData);
}

