#include "stdafx.h"
#include "EngineCenter.h"
#include "Render/RenderObjectManager.h"
#include "Render/AnimationManager.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/ResourcePathManager.h"
#include "Render/renderer.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentManager.h"
#include "Utility/BeatsUtility/StringHelper.h"
#include "Utility/BeatsUtility/EnumStrGenerator.h"
#include "Utility/PerformDetector/PerformDetector.h"
#include "GUI/System.h"
#include "GUI/Animation/AnimationManager_ui.h"
#include "GUI/Font/FontManager.h"
#include "Render/TextureFragManager.h"
#include "Render/RenderGroupManager.h"
#include "GUI/WindowManager.h"
#include "LanguageManager.h"
#include "Render/TextureFormatConverter.h"
#include "ParticlesSystem/ParticleSystemManager.h"
#include "SkeletalAnimation/CAnimationManager.h"
#include "SkeletalAnimation/CAnimationImporter.h"
#include "external/CConfiguration.h"
#include "external/CFileUnitils.h"
#include "Event/CTouchDelegate.h"

CEngineCenter* CEngineCenter::m_pInstance = NULL;

CEngineCenter::CEngineCenter()
    : m_uFPS(0)
    , m_uFrameTime(0)
    , m_uDeltaTimeForThisUpdate(0)
    , m_uUpdateCounter(0)
    , m_uShouldUpdateCounter(0)
    , m_uMakeUpTime(0)
{
    m_uLastUpdateTime.QuadPart = 0;
    m_uCurrentTime.QuadPart = 0;
    QueryPerformanceFrequency(&m_uTimeFrequency);
    SetFPS(60);
}

CEngineCenter::~CEngineCenter()
{
    CPerformDetector::Destroy();
    FCEngine::CParticleSystemManager::Destroy();
    _2DSkeletalAnimation::CAnimationImporter::Destroy();
    _2DSkeletalAnimation::CAnimationManager::Destroy();
    CRenderObjectManager::Destroy();
    CRenderGroupManager::Destroy();
    CTextureFragManager::Destroy();
    CRenderManager::Destroy();
    CResourcePathManager::Destroy();
    CComponentManager::Destroy();
    CEnumStrGenerator::Destroy();
    FCGUI::CAnimationManager::Destroy();
    FCGUI::CWindowManager::Destroy();
    FCGUI::CSystem::Destroy();
    FCGUI::CFontManager::Destroy();
    CStringHelper::Destroy();
    CResourceManager::Destroy();
    CAnimationManager::Destroy();
    CRenderer::Destroy();
    CLanguageManager::Destroy();
    CTextureFormatConverter::Destroy();
    CFileUtils::Destroy();
    CConfingation::Destroy();
    CTouchDelegate::Destroy();

#ifdef _DEBUG
    SymCleanup(GetCurrentProcess());
#endif
}

bool CEngineCenter::Initialize(size_t uWindowWidth/* = 0xFFFFFFFF*/, size_t uWindowHeight/* = 0xFFFFFFFF*/)
{
#ifdef _DEBUG
    SymSetOptions(SYMOPT_LOAD_LINES);
    bool bInitializeSuccess = SymInitialize(GetCurrentProcess(), NULL, TRUE) == TRUE;
    BEATS_ASSERT(bInitializeSuccess);
#endif

    const TString& strSourceCodePath = CResourcePathManager::GetInstance()->GetResourcePath(CResourcePathManager::eRPT_SourceCode);
    std::vector<TString> scanPath;
    scanPath.push_back(strSourceCodePath);
    CEnumStrGenerator::GetInstance()->Init(scanPath);
    CPerformDetector::GetInstance()->SetTypeName(pszPerformNodeStr, sizeof(pszPerformNodeStr));

    if (0xFFFFFFFF == uWindowWidth && 0xFFFFFFFF == uWindowHeight)
    {
        CRenderManager::GetInstance()->Initialize();
    }
    else
    {
        CRenderManager::GetInstance()->InitializeWithWindow(uWindowWidth, uWindowHeight);
    }
    FCGUI::CWindowManager::GetInstance()->Init();
    FCGUI::CSystem::GetInstance()->Init();
    pComponentLauncherFunc();
    return true;
}

void CEngineCenter::Update()
{
    if ( ShouldUpdateThisFrame())
    {
        QueryPerformanceCounter(&m_uLastUpdateTime);
        float dt = (float)GetDeltaTime() / m_uTimeFrequency.QuadPart;
        FC_PERFORMDETECT_START(ePNT_AnimationManager)
        CAnimationManager::GetInstance()->Update(dt);
        FC_PERFORMDETECT_STOP(ePNT_AnimationManager)

        FC_PERFORMDETECT_START(ePNT_GUI_AnimationManager)
        FCGUI::CAnimationManager::GetInstance()->Update(dt);
        FC_PERFORMDETECT_STOP(ePNT_GUI_AnimationManager)

        FC_PERFORMDETECT_START(ePNT_2DSkeleton_AnimationManager)
        _2DSkeletalAnimation::CAnimationManager::GetInstance()->Update(dt);
        FC_PERFORMDETECT_STOP(ePNT_2DSkeleton_AnimationManager)

        FC_PERFORMDETECT_START(ePNT_ParticleSystemManager)
        FCEngine::CParticleSystemManager::GetInstance()->Update(dt);
        FC_PERFORMDETECT_STOP(ePNT_ParticleSystemManager)

        FC_PERFORMDETECT_START(ePNT_Render)
        CRenderManager::GetInstance()->Render();
        FC_PERFORMDETECT_STOP(ePNT_Render)
    }
    else
    {
        Sleep(0);
    }
}

void CEngineCenter::SetFPS(size_t uFramePerSecond)
{
    m_uFPS = uFramePerSecond;
    // TODO: Add 2 to the FPS for compensation, I don't know why.
    m_uFrameTime = (size_t)((float)m_uTimeFrequency.QuadPart / (m_uFPS + 2));
}

void CEngineCenter::IncreaseUpdateCounter()
{
    m_uUpdateCounter++;
}

bool CEngineCenter::ShouldUpdateThisFrame()
{
    bool bRet = (m_uShouldUpdateCounter == m_uUpdateCounter) && m_uShouldUpdateCounter != 0;
    if (!bRet)
    {
        QueryPerformanceCounter(&m_uCurrentTime);
        if (m_uLastUpdateTime.QuadPart == 0)
        {
            bRet = true;
            m_uDeltaTimeForThisUpdate = m_uFrameTime;
        }
        else
        {
            size_t uTimeOffSet = (size_t)(m_uCurrentTime.QuadPart - m_uLastUpdateTime.QuadPart);
            if (uTimeOffSet + m_uMakeUpTime >= m_uFrameTime)
            {
                m_uMakeUpTime = m_uMakeUpTime + uTimeOffSet - m_uFrameTime;
                m_uDeltaTimeForThisUpdate = uTimeOffSet;
                bRet = true;
            }
        }

        if (bRet)
        {
            m_uShouldUpdateCounter = m_uUpdateCounter;
        }
    }
    return bRet;
}

size_t CEngineCenter::GetDeltaTime() const
{
    return m_uDeltaTimeForThisUpdate;
}

