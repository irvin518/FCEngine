#include "stdafx.h"
#include "CAnimationController.h"
#include "CMoveData.h"
#include "CBoneData.h"
#include "CArmatureData.h"
#include "CAnimationData.h"
#include "CAnimationManager.h"
#include "CAnimationModel.h"
#include <math.h>
#include "Utility/BeatsUtility/StringHelper.h"

using namespace _2DSkeletalAnimation;

CAnimationController::CAnimationController(CAnimationModel* pModel)
    : m_bLoop(true)
    , m_uLoopCount(0)
    , m_bPlaying(false)
    , m_fPlayingTime(0.f)
    , m_fBlendTime(0.5f)
    , m_fDuration(0.f)
    , m_uFps(24)
    , m_bHaveRender(false)
    , m_pModel(pModel)
    , m_pPreMove(nullptr)
    , m_pCurrMove(nullptr)
{
}

CAnimationController::~CAnimationController()
{
}

const std::string& CAnimationController::GetAnimationName() const
{
    return m_strAnimationName;
}

const std::string& CAnimationController::GetArmatureName() const
{
    return m_strArmatureName;
}

void CAnimationController::SetAnimationName(const std::string& name)
{
    m_strAnimationName = name;
}

void CAnimationController::SetArmatureName(const std::string& name)
{
    m_strArmatureName = name;
}

void CAnimationController::PlayMove(const TString& moveName)
{
    CAnimationData * pAniData =_2DSkeletalAnimation::CAnimationManager::GetInstance()->GetAnimationData(m_strAnimationName);
    size_t len = moveName.size() * 2 + 1;
    char *movename = new char[len];
    CStringHelper::GetInstance()->ConvertToCHAR(moveName.c_str(), movename, len);
    CMoveData* pMoveData = pAniData->GetMovData(movename);
    BEATS_SAFE_DELETE_ARRAY(movename);
    PlayMove(pMoveData);
}

void CAnimationController::PlayMove(CMoveData* pMoveData)
{
    BEATS_ASSERT(pMoveData);
    m_pPreMove = m_pCurrMove;
    m_pCurrMove = pMoveData;
    m_bLoop = m_pCurrMove->GetLoop();
    m_fDuration = (float)(m_pCurrMove->GetDuration()) / (float)m_uFps;
    m_uFrameCount = (size_t)m_pCurrMove->GetDuration();
    if(!m_bHaveRender)
    {
        m_pModel->RenderSkeletal();
        m_bHaveRender = true;
    }

    Play();
}

const CMoveData* CAnimationController::GetCurrMove() const
{
    return m_pCurrMove;
}

void CAnimationController::Play()
{
    m_fPlayingTime = 0.f;
    m_bPlaying = true;
    m_uLoopCount = 0;
}

void CAnimationController::Update(float fDeltaTime)
{
    if(m_bPlaying)
    {
        m_fPlayingTime += fDeltaTime;
        if (m_fPlayingTime > m_fDuration)
        {
            if(m_bLoop)
            {
                m_fPlayingTime = fmod(m_fPlayingTime, m_fDuration);
                m_uLoopCount++;
            }
            else
            {
                m_fPlayingTime = m_fDuration;
            }
        }

        m_pModel->Update();
    }
}

void CAnimationController::GoToFrame(size_t frame)
{
    if(m_pCurrMove)
    {
        m_fPlayingTime = (float)frame / m_uFps;
        m_bPlaying = false;
    }
}
void CAnimationController::Pause()
{
    m_bPlaying= false;
}
void CAnimationController::Resume()
{
    if(m_pCurrMove)
    {
        m_bPlaying = true;
    }
}
void CAnimationController::Stop()
{
    m_fPlayingTime = 0;
    m_bPlaying = false;
}
bool CAnimationController::IsPlaying()
{
    return m_bPlaying;
}

size_t CAnimationController::GetCurrFrame()
{
    size_t iFrame = (size_t)(m_uFps * m_fPlayingTime);
    if(iFrame >= m_uFrameCount)
        iFrame = m_uFrameCount -1;
    return iFrame;
}

const std::vector<std::string>& CAnimationController::GetBoneNames()
{
    m_boneNames.clear();
    CArmatureData* armature = CAnimationManager::GetInstance()->GetArmatureData(m_strArmatureName);
    const std::vector<CBoneData*>&bones = armature->GetBoneOrderList();

    for(auto &boneitem : bones)
    {
        m_boneNames.push_back(boneitem->GetName());
    }
    return m_boneNames;
}

CAnimationController::TFrameDataMap& CAnimationController::GetInitBonesData()
{
    GetFrameDataByFrame(m_frameDataMap,m_pCurrMove, 0);
    return m_frameDataMap;
}

CAnimationController::TFrameDataMap& CAnimationController::GetCurrFrameBonesData()
{
    m_frameDataMap.clear();
    if(IsBlendState())
    {
        TFrameDataMap preFrameDataMap;
        TFrameDataMap currFrameDataMap;
        GetMoveFrameData(preFrameDataMap, m_pPreMove);
        if(preFrameDataMap.empty())
        {
            GetMoveFrameData(m_frameDataMap, m_pCurrMove);
        }
        else
        {
            GetMoveFrameData(currFrameDataMap, m_pCurrMove);
            Interpolation(preFrameDataMap, currFrameDataMap);
        }
    }
    else
    {
        GetMoveFrameData(m_frameDataMap, m_pCurrMove);
    }
    return m_frameDataMap;
}

void CAnimationController::InsertFrameData(SFrameData& result,  const SFrameData& beforeData, const SFrameData& afterData, int iFrame)
{
    float percent = (float)(iFrame - beforeData.frameID) / (float)(afterData.frameID - beforeData.frameID);
    result.frameID = iFrame;
    result.x = beforeData.x + ( afterData.x - beforeData.x) * percent;
    result.y = beforeData.y + ( afterData.y - beforeData.y) * percent;

    result.skewX = beforeData.skewX + ( afterData.skewX - beforeData.skewX) * percent;
    result.skewY = beforeData.skewY + ( afterData.skewY - beforeData.skewY) * percent;

    result.scaleX = beforeData.scaleX + ( afterData.scaleX - beforeData.scaleX) * percent;
    result.scaleY = beforeData.scaleY + ( afterData.scaleY - beforeData.scaleY) * percent;
}

bool CAnimationController::IsBlendState()
{
    return m_pPreMove && m_pCurrMove && m_fPlayingTime < m_fBlendTime 
        && m_uLoopCount < 1;
}

void CAnimationController::GetMoveFrameData(TFrameDataMap& frameDataMap, CMoveData* pMoveData)
{
    return GetFrameDataByFrame(frameDataMap,pMoveData,GetCurrFrame());
}

void CAnimationController::GetFrameDataByFrame(TFrameDataMap& frameDataMap, CMoveData* pMoveData, int iFrame)
{
    BEATS_ASSERT(pMoveData);
    frameDataMap.clear();
    CArmatureData* armature = CAnimationManager::GetInstance()->GetArmatureData(m_strArmatureName);
    const std::vector<CBoneData*>&bones = armature->GetBoneOrderList();
    for(auto &boneitem : bones)
    {
        std::string boneName = boneitem->GetName();
        SMoveBoneData* pBoneData = pMoveData->GetMovBoneData(boneName);
        if(pBoneData != nullptr)
        {
            for(size_t i = 0; i<pBoneData->frameList.size(); i++)
            {
                SFrameData* framedata = pBoneData->frameList[i];
                SFrameData* next =nullptr;
                if( i +1 < pBoneData->frameList.size())
                {
                    next = pBoneData->frameList[i+1];
                }

                if(framedata->frameID == iFrame && framedata->dI != -1)
                {
                    SFrameData data;
                    data.dI = framedata->dI;
                    data.frameID = framedata->frameID;
                    data.x = framedata->x;
                    data.y = framedata->y;
                    data.skewX = framedata->skewX;
                    data.skewY = framedata->skewY;
                    data.scaleX = framedata->scaleX;
                    data.scaleY = framedata->scaleY;
                    frameDataMap[boneName] = data;
                    break;
                }
                else if(boneName.compare("effect") != 0 && iFrame > framedata->frameID 
                    && next && next->frameID > iFrame)
                {
                    SFrameData data;
                    data.dI = framedata->dI;
                    InsertFrameData(data,  *framedata, *next, iFrame);
                    frameDataMap[boneName] = data;
                    break;
                }
            }
        }
    }
}

void CAnimationController::Interpolation(const TFrameDataMap& preFrameDataMap, const TFrameDataMap& currFrameDataMap)
{
    float fWeight = m_fPlayingTime / m_fBlendTime;
    for (auto& item : preFrameDataMap)
    {
        const SFrameData& preFrameData = item.second;
        TFrameDataMap ::const_iterator iter = currFrameDataMap.find(item.first);
        if (iter != currFrameDataMap.end())
        {
            const SFrameData& currFrameData = iter->second;
            SFrameData framedata;
            framedata.dI = currFrameData.dI;
            framedata.frameID = currFrameData.frameID;
            framedata.x = preFrameData.x + ( currFrameData.x - preFrameData.x) * fWeight;
            framedata.y = preFrameData.y + ( currFrameData.y - preFrameData.y) * fWeight;
            framedata.scaleX = preFrameData.scaleX + (currFrameData.scaleX - preFrameData.scaleX) * fWeight;
            framedata.scaleY = preFrameData.scaleY + (currFrameData.scaleY - preFrameData.scaleY) * fWeight;
            framedata.skewX = preFrameData.skewX + (currFrameData.skewX - preFrameData.skewX) * fWeight;
            framedata.skewY = preFrameData.skewY + (currFrameData.skewY - preFrameData.skewY) * fWeight;
            m_frameDataMap[item.first] = framedata;
        }
    }
}

