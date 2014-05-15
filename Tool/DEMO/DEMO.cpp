// EngineSample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DEMO.h"
#include "GL/glew.h"
#include "glfw3.h"
#include "CCConsole.h"
#include "CCGeometry.h"
#include "PublicDef.h"
#include "Render/RenderPublic.h"
#include "Resource/ResourcePublic.h"
#include "Utility/BeatsUtility/SharePtr.h"
#include "Render/RenderManager.h"
#include "Render/RenderObjectManager.h"
#include "EngineCenter.h"
#include "Render/ShaderProgram.h"
#include "Render/Texture.h"
#include "Resource/ResourcePublic.h"
#include "Resource/ResourceManager.h"
#include "Render/Model.h"
#include "Render/AnimationManager.h"
#include "Render/Sprite.h"
#include "Render/Skin.h"
#include "Render/Skeleton.h"
#include "Render/Animation.h"
#include "Render/Camera.h"
#define  SHOW_UI_CONTROL
#include <mmsystem.h>
#ifdef SHOW_UI_CONTROL
#include "GUI/System.h"
#include "GUI/Window/Window.h"
#include "GUI/Renderer/BaseRenderer.h"
#include "GUI/WindowManager.h"
#include "GUI/Event/WindowEvent.h"
#include "GUI/Window/Button.h"
#include "GUI/Layout/GridLayout.h"
#include "GUI/Layout/ScrollLayout.h"
#include "GUI/Layout/VScrollLayout.h"
#include "GUI/Layout/HScrollLayout.h"
#include "Event/MouseEvent.h"
#include "Render/TextureFragManager.h"
#include "GUI/Window/CheckBox.h"
#include "GUI/Window/Slider.h"
#include "GUI/Window/Progress.h"
#include "GUI/Window/TextEdit.h"
#include "GUI/Window/TextLabel.h"
#include "GUI/Window/ListBox.h"
#include "GUI/Window/ComboBox.h"
#include "GUI/Window/ListControl.h"
#include "GUI/Renderer/SliderRenderer.h"
#include "GUI/Renderer/CheckBoxRenderer.h"
#include "GUI/Renderer/ProgressRenderer.h"
#include "GUI/Renderer/TextEditRenderer.h"
#include "GUI/Renderer/TextLabelRenderer.h"
#include "GUI/Renderer/ListBoxRenderer.h"
#include "GUI/Renderer/ComboBoxRenderer.h"

void TestUI();
#endif

static const size_t Window_Width = 800;
static const size_t Window_Height = 600;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       /*nCmdShow*/)
{
    MSG msg;
    {
        ::ZeroMemory(&msg, sizeof(MSG));
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(lpCmdLine);
        HACCEL hAccelTable;

        CEngineCenter::GetInstance()->Initialize(Window_Width, Window_Height);

        hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

        CModel *model = CRenderObjectManager::GetInstance()->CreateModel();

        SharePtr<CTexture> pTestTexture( 
            CResourceManager::GetInstance()->GetResource<CTexture>(_T("TestTexture.tga"), false));
        model->AddTexture(pTestTexture);

        SharePtr<CSkin> pSkin(
            CResourceManager::GetInstance()->GetResource<CSkin>(
            _T("org.skin"), false));
        model->SetSkin(pSkin);

        SharePtr<CSkeleton> pSkeleton(
            CResourceManager::GetInstance()->GetResource<CSkeleton>(
            _T("org.ske"), false));
        model->SetSkeleton(pSkeleton);

        SharePtr<CAnimation> pAnimation(
            CResourceManager::GetInstance()->GetResource<CAnimation>(
            _T("org.ani"), false));
        model->SetAnimaton(pAnimation);
        model->PlayAnimationById(0, 0.f, true);
        CCamera* pCamera = new CCamera(); //CCamera::eCT_2D
        pCamera->SetWidth(Window_Width);
        pCamera->SetHeight(Window_Height);
        // Main message loop:
        while (!glfwWindowShouldClose(CRenderManager::GetInstance()->GetMainWindow()))
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
            {
                if (GetMessage(&msg, NULL, 0, 0))
                {
                    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    } 
                }
            }
            else
            {
                CRenderManager::GetInstance()->SetCamera(pCamera);
                CEngineCenter::GetInstance()->Update();
                CRenderManager::GetInstance()->SetCamera(pCamera);
            }
        }
        BEATS_SAFE_DELETE(pCamera);
    }
    CEngineCenter::Destroy();
    return (int) msg.wParam;
}


