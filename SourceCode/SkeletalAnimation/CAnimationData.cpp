#include "stdafx.h"
#include "CAnimationData.h"
#include "CMoveData.h"
using namespace _2DSkeletalAnimation;

CAnimationData::CAnimationData()
{

}

CAnimationData::~CAnimationData()
{
    for (auto &item : m_movedataMap)
    {
        CMoveData* pMoveData = item.second;
        BEATS_SAFE_DELETE(pMoveData);
    }
}

void CAnimationData::AddMoveData(CMoveData *moveData)
{
    m_movedataMap[moveData->GetName()] = moveData;
}

CMoveData *CAnimationData::GetMovData(const std::string& moveName)
{
    CMoveData* pRet = nullptr;
    MoveDataMap ::const_iterator iter = m_movedataMap.find(moveName);
    BEATS_ASSERT(iter != m_movedataMap.end());
    if (iter != m_movedataMap.end())
    {
        pRet = iter->second;
    }
    return pRet;
}

void CAnimationData::SetName(const std::string name)
{
    m_strName = name;
}

const std::string& CAnimationData::GetName() const
{
    return m_strName;
}

const std::vector<std::string>& CAnimationData::GetMoveNameList()
{
    if(m_moveNameList.empty())
    {
        for (auto &item : m_movedataMap)
        {
            m_moveNameList.push_back(item.first);
        }
    }

    return m_moveNameList;
}
