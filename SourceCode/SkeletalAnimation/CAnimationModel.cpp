#include "stdafx.h"
#include "CAnimationModel.h"
#include "CAnimationManager.h"
#include "CAnimationController.h"
#include "GUI/Window/Window.h"
#include "GUI/WindowManager.h"
#include "GUI/Renderer/BaseRenderer.h"
#include "Render/TextureFragManager.h"
#include "Render/RenderManager.h"
#include "Render/Texture.h"
#include "Resource/ResourcePublic.h"
#include "Resource/ResourceManager.h"

#include "CAnimationData.h"
#include "CArmatureData.h"
#include "CMoveData.h"
#include "CTextureData.h"
#include "CBoneData.h"
#include "Utility/BeatsUtility/StringHelper.h"
#include "Render/TextureAtlas.h"

using namespace _2DSkeletalAnimation;
using namespace FCGUI;

CAnimationModel::CAnimationModel()
{
    Init();
    Load();
}

void CAnimationModel::Init()
{
    m_bPlayAnimation = false;
    m_strResourcePath = CResourcePathManager::GetInstance()->GetResourcePath(CResourcePathManager::eRPT_Resource)+_T("SkeletalAnimation/");
    m_strCfgFilePath.m_value = _T("tauren.ExportJson");
    m_pAnimationController = CAnimationManager::GetInstance()->CreateController(this);
}

CAnimationModel::~CAnimationModel()
{
    TCHAR szMode[MAX_PATH];
    CStringHelper::GetInstance()->ConvertToTCHAR(m_pAnimationController->GetAnimationName().c_str(), szMode, MAX_PATH);
    CWindow *pModel = CWindowManager::GetInstance()->GetWindow(szMode);
    BEATS_ASSERT(pModel != nullptr);
    CWindowManager::GetInstance()->DestroyWindow(pModel);
    CAnimationManager::GetInstance()->DeleteController(m_pAnimationController);
}

void CAnimationModel::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_bPlayAnimation, true, 0xFFFFFFFF, _T("是否播放动画"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strCfgFilePath, true, 0xFFFFFFFF, _T("文件"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strMoveName, true, 0xFFFFFFFF, _T("动画名称"), NULL, NULL, GEN_ENUM_PARAM(m_moveNameList, NULL));
}

bool CAnimationModel::OnPropertyChange(void* pVariableAddr, void* pNewValueToBeSet)
{
    bool bRet = super::OnPropertyChange(pVariableAddr, pNewValueToBeSet);
    if (!bRet)
    {
        if(pVariableAddr == &m_bPlayAnimation)
        {
            DeserializeVarialble(m_bPlayAnimation, (CSerializer*) pNewValueToBeSet);
            if(m_bPlayAnimation)
            {
                m_pAnimationController->PlayMove(m_strMoveName);
            }
            else
            {
                m_pAnimationController->Stop();
            }
            bRet = true;
        }
        else if(pVariableAddr == &m_strCfgFilePath)
        {
            DeserializeVarialble(m_strCfgFilePath, (CSerializer*) pNewValueToBeSet);
            Load();

            CComponentEditorProxy* pProxy = static_cast<CComponentEditorProxy*>(CComponentProxyManager::GetInstance()->GetComponentInstance(this->GetId(), this->GetGuid()));
            BEATS_ASSERT(pProxy != NULL, _T("Can't get proxy with id: %d guid:0x%x"), GetId(), GetGuid());
            CPropertyDescriptionBase* pProperty = pProxy->GetPropertyDescription(GET_VAR_NAME(m_strMoveName));
            BEATS_ASSERT(pProperty != NULL, _T("Get property %s failed!"), GET_VAR_NAME(m_strMoveName));
            pProperty->SetValueList(m_moveNameList);
            bRet = true;
        }
        else if(pVariableAddr == &m_strMoveName)
        {
            DeserializeVarialble(m_strMoveName, (CSerializer*) pNewValueToBeSet);
            m_pAnimationController->PlayMove(m_strMoveName);
            bRet = true;
        }
    }
    return bRet;
}

void CAnimationModel::Load()
{
    CAnimationManager::GetInstance()->AddAnimationDataFromFile(m_strResourcePath + m_strCfgFilePath.m_value);
    const std::string& animationName = CAnimationManager::GetInstance()->GetLastAnimationName();
    const std::string& armatureName = CAnimationManager::GetInstance()->GetLastArmatureName();
    m_pAnimationController->SetAnimationName(animationName);
    m_pAnimationController->SetArmatureName(armatureName);

    CAnimationData* pAnimationData = CAnimationManager::GetInstance()->GetAnimationData(animationName);
    const std::vector<std::string> moveNamelist = pAnimationData->GetMoveNameList();
    for (auto &item : moveNamelist)
    {
        size_t len = item.size() + 1;
        TCHAR *movename = new TCHAR[len];
        CStringHelper::GetInstance()->ConvertToTCHAR(item.c_str(), movename, len);
        m_moveNameList.push_back(movename);

        if(m_strMoveName.empty())
            m_strMoveName = movename;
        BEATS_SAFE_DELETE_ARRAY(movename);
    }
}

void CAnimationModel::RenderSkeletal()
{
    TCHAR szMode[MAX_PATH];
    CStringHelper::GetInstance()->ConvertToTCHAR(m_pAnimationController->GetAnimationName().c_str(), szMode, MAX_PATH);
    CWindow *model = CWindowManager::GetInstance()->Create<CWindow>(szMode, nullptr);
    model->SetPosSize(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
    model->SetRenderer(new CBaseRenderer());
    std::map<std::string, SFrameData> framedatas = m_pAnimationController->GetInitBonesData();
    std::vector<std::string> names =  m_pAnimationController->GetBoneNames();
    for (auto& item : names)
    {
        CArmatureData* pArmature = CAnimationManager::GetInstance()->GetArmatureData(m_pAnimationController->GetArmatureName());
        const CBoneData* pBoneData = pArmature->GetBoneData(item.c_str());
        const std::vector<SDisplayData>& displayList = pBoneData->GetDisplayDataList();
        for (auto &displaydata : displayList)
        {
            TCHAR szTextureName[MAX_PATH];
            CStringHelper::GetInstance()->ConvertToTCHAR(displaydata.name.c_str(), szTextureName, MAX_PATH);

            CWindow*  pBone =   CWindowManager::GetInstance()->Create<CWindow>(szTextureName, model);
            pBone->SetAnchor(0.5f, 0.5f);
            pBone->SetPercentPos(0.5, 0.5);
            pBone->SetVisible(false);

            TString fragName = _T("tauren.xml/");
            fragName += szTextureName;
            pBone->Renderer()->AddLayer(fragName);

            std::map<std::string, SFrameData>::const_iterator iter = framedatas.find(item);
            if(iter != framedatas.end())
            {
                pBone->SetVisible(true);
                const SFrameData& framedata = iter->second;
                pBone->SetPos(framedata.x, -framedata.y);
                pBone->SetScale(framedata.scaleX, framedata.scaleY);
                kmScalar skewX = framedata.skewX +displaydata.skindata.skewX;
                pBone->SetRotation(kmRadiansToDegrees(skewX));
            }
        }
    }
}

void CAnimationModel::Update()
{
    std::map<std::string, SFrameData> framedatas = m_pAnimationController->GetCurrFrameBonesData();
    std::vector<std::string> names =  m_pAnimationController->GetBoneNames();
    for (auto& bonename : names)
    {
        TCHAR szBoneName[MAX_PATH];
        CStringHelper::GetInstance()->ConvertToTCHAR(bonename.c_str(), szBoneName, MAX_PATH);
        CArmatureData* pArmature = CAnimationManager::GetInstance()->GetArmatureData(m_pAnimationController->GetArmatureName());
        const CBoneData* pBoneData = pArmature->GetBoneData(bonename.c_str());
        const std::vector<SDisplayData>& displayList = pBoneData->GetDisplayDataList();

        for (size_t i = 0; i<displayList.size(); ++i)
        {
            SDisplayData displaydata = displayList[i];
            TCHAR szTextureName[MAX_PATH];
            CStringHelper::GetInstance()->ConvertToTCHAR(displaydata.name.c_str(), szTextureName, MAX_PATH);
            CWindow*  pBone =   CWindowManager::GetInstance()->GetWindow(szTextureName);
            BEATS_ASSERT(pBone);
            std::map<std::string, SFrameData>::const_iterator iter = framedatas.find(bonename);
            if(iter != framedatas.end())
            {
                const SFrameData& framedata = iter->second;
                bool bVisible = (size_t)framedata.dI == i;
                pBone->SetVisible(bVisible);
                if(bVisible)
                {
                    pBone->SetPos(framedata.x, -framedata.y);
                    pBone->SetScale(framedata.scaleX, framedata.scaleY);
                    kmScalar skewX = framedata.skewX + displaydata.skindata.skewX;
                    pBone->SetRotation(kmRadiansToDegrees(skewX));
                }

            }
            else
            {
                pBone->SetVisible(false);
            }
        }
    }
}

void CAnimationModel::ReplaceDisplayData(const std::string& boneName, const SDisplayData& displayData)
{
     CArmatureData* pArmature = CAnimationManager::GetInstance()->GetArmatureData(m_pAnimationController->GetArmatureName());
     SDisplayData oldDisplayData = pArmature->ReplaceDisplayData(boneName,displayData);

     TCHAR szOldTextureName[MAX_PATH];
     CStringHelper::GetInstance()->ConvertToTCHAR(oldDisplayData.name.c_str(), szOldTextureName, MAX_PATH);
     CWindowManager::GetInstance()->DestroyWindow(szOldTextureName);

     TCHAR szMode[MAX_PATH];
     CStringHelper::GetInstance()->ConvertToTCHAR(m_pAnimationController->GetAnimationName().c_str(), szMode, MAX_PATH);
     CWindow* pModel = CWindowManager::GetInstance()->GetWindow(szMode);
     TCHAR szTextureName[MAX_PATH];
     CStringHelper::GetInstance()->ConvertToTCHAR(displayData.name.c_str(), szTextureName, MAX_PATH);
     CWindow*  pBone =   CWindowManager::GetInstance()->Create<CWindow>(szTextureName, pModel);
     pBone->SetAnchor(0.5f, 0.5f);
     pBone->SetPercentPos(0.5, 0.5);
     pBone->SetVisible(false);
}

