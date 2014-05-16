// DEMO.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DEMO.h"
#include "TestFan.h"
#include "GLES-Render.h"
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
#include "Render/Sprite.h"
#include "Render/TextureAtlas.h"
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

void Render();

#endif

static const size_t Window_Width = 960;
static const size_t Window_Height = 640;

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

        SharePtr<CTexture> texture = CResourceManager::GetInstance()->GetResource<CTexture>(_T("testtank.png"), false);
        CResourceManager::GetInstance()->GetResource<CTextureAtlas>(_T("testtank.xml"),false);
#ifdef SHOW_UI_CONTROL
        TestUI();
#endif
        CCamera* pCamera = new CCamera(CCamera::eCT_2D);
        pCamera->SetWidth(Window_Width);
        pCamera->SetHeight(Window_Height);

//       CTestFan* _pTestFan = new CTestFan();
//        CRenderObjectManager::GetInstance()->AddRenderObject((CRenderObject*)_pTestFan);

        kmVec2 testScene;
        testScene.x = 1280;
        testScene.y = 640;
//         CSprite* _pTestS = new CSprite( _T("testtank"), _T("scene1"), testScene );
//         CRenderObjectManager::GetInstance()->AddSprite(_pTestS);
        CRenderObjectManager::GetInstance()->CreateSprite( _T("testtank"), _T("scene1"), testScene );
        testScene.x = 128;
        testScene.y = 128;
//         CSprite* _pTestT = new CSprite( _T("testtank"), _T("NewAnimation0"), testScene );
//         CRenderObjectManager::GetInstance()->AddSprite(_pTestT);
        CRenderObjectManager::GetInstance()->CreateSprite( _T("testtank"), _T("NewAnimation0"), testScene );
        testScene.x = 128;
        testScene.y = 128;
//         CSprite* _pTestP = new CSprite( _T("testtank"), _T("NewAnimation20"), testScene );
//         CRenderObjectManager::GetInstance()->AddSprite(_pTestP);
        CRenderObjectManager::GetInstance()->CreateSprite( _T("testtank"), _T("NewAnimation20"), testScene );


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
#ifdef SHOW_UI_CONTROL
                FCGUI::CSystem::GetInstance()->Update((float)300/1000);
#endif
                CRenderManager::GetInstance()->SetCamera(pCamera);
                CEngineCenter::GetInstance()->IncreaseUpdateCounter();
            }
        }
        BEATS_SAFE_DELETE(pCamera);
    }
    CEngineCenter::Destroy();
    return (int) msg.wParam;
}

#ifdef SHOW_UI_CONTROL
void OnSelChange(CBaseEvent *event);
void OnClick(CBaseEvent *event);

void TestUI()
{
//     SharePtr<CTexture> texture = 
//         CResourceManager::GetInstance()->GetResource<CTexture>(_T("DemoShop0.png"), false);
//     CResourceManager::GetInstance()->GetResource<CTextureAtlas>(_T("TestGUI.xml"),false);
    using namespace FCGUI;
    
    CWindow *wndBG = CWindowManager::GetInstance()->Create<CWindow>(_T("TestLRockerBG"), nullptr);
    wndBG->SetPosSize(0.f, 0.f, 0.2f, 0.8f, 146.f, 146.f, 0.f, 0.f);
    wndBG->SetRenderer(new CBaseRenderer());
    wndBG->Renderer()->AddLayer(_T("testtank.xml/Control_Back"));
    wndBG->SetAnchor(0.5f, 0.5f);

    CButton *pLButton = CWindowManager::GetInstance()->Create<CButton>(_T("TestLRocker"), wndBG);
    pLButton->SetPosSize(0.f, 0.f, 0.5f, 0.5f, 86.f, 86.f, 0.f, 0.f);
    pLButton->SetRenderer(new CBaseRenderer());
    pLButton->Renderer()->AddLayer(_T("testtank.xml/Control_Button_down"),CButton::eST_STATE_PRESSED);
    pLButton->Renderer()->AddLayer(_T("testtank.xml/Control_Button"),CButton::eST_STATE_NORMAL);
    pLButton->SetAnchor(0.5f, 0.5f);
    pLButton->SubscribeEvent(CButton::eET_EVENT_CLICKED, OnClick);


    CWindow *wndRBG = CWindowManager::GetInstance()->Create<CWindow>(_T("TestRRockerBG"), nullptr);
    wndRBG->SetPosSize(0.f, 0.f, 0.8f, 0.8f, 146.f, 146.f, 0.f, 0.f);
    wndRBG->SetRenderer(new CBaseRenderer());
    wndRBG->Renderer()->AddLayer(_T("testtank.xml/Control_Back"));
    wndRBG->SetAnchor(0.5f, 0.5f);


    CButton *pRButton = CWindowManager::GetInstance()->Create<CButton>(_T("TestRRocker"), wndRBG);
    pRButton->SetPosSize(0.f, 0.f, 0.5f, 0.5f, 86.f, 86.f, 0.f, 0.f);
    pRButton->SetRenderer(new CBaseRenderer());
    pRButton->Renderer()->AddLayer(_T("testtank.xml/Control_Button_down"),CButton::eST_STATE_PRESSED);
    pRButton->Renderer()->AddLayer(_T("testtank.xml/Control_Button"),CButton::eST_STATE_NORMAL);
    pRButton->SetAnchor(0.5f, 0.5f);
    pRButton->SubscribeEvent(CButton::eET_EVENT_CLICKED, OnClick);

    
    CWindow *wndHBG = CWindowManager::GetInstance()->Create<CWindow>(_T("TestHeadBG"), nullptr);
    wndHBG->SetPosSize(0.f, 0.f, 0.2f, 0.1f, 100.f, 200.f, 0.f, 0.f);
    wndHBG->SetAnchor(0.5f, 0.5f);

    CWindow *wndHeadB = CWindowManager::GetInstance()->Create<CWindow>(_T("TestHeadB"), wndHBG);
    wndHeadB->SetPosSize(0.f, 0.f, 0.1f, 0.4f, 98.f, 72.f, 0.f, 0.f);
    wndHeadB->SetRenderer(new CBaseRenderer());
    wndHeadB->Renderer()->AddLayer(_T("testtank.xml/MapMain"));
    wndHeadB->SetAnchor(0.5f, 0.5f);

    CWindow *wndHead = CWindowManager::GetInstance()->Create<CWindow>(_T("TestHeadX"), wndHBG);
    wndHead->SetPosSize(0.f, 0.f, 0.1f, 0.4f, 85.f, 85.f, 0.f, 0.f);
    wndHead->SetRenderer(new CBaseRenderer());
    wndHead->Renderer()->AddLayer(_T("testtank.xml/LittleHelper"));
    wndHead->SetAnchor(0.5f, 0.5f);

    CWindow *wndHPBG = CWindowManager::GetInstance()->Create<CWindow>(_T("TestHPBG"), wndHBG);
    wndHPBG->SetPosSize(0.f, 0.f, 1.22f, 0.4f, 136.f, 20.f, 0.f, 0.f);
    wndHPBG->SetRenderer(new CBaseRenderer());
    wndHPBG->Renderer()->AddLayer(_T("testtank.xml/MainSlot"));
    wndHPBG->SetAnchor(0.5f, 0.5f);
    CWindow *wndHP = CWindowManager::GetInstance()->Create<CWindow>(_T("TestHP"), wndHBG);
    wndHP->SetPosSize(0.f, 0.f, 1.22f, 0.4f, 130.f, 15.f, 0.f, 0.f);
    wndHP->SetRenderer(new CBaseRenderer());
    wndHP->Renderer()->AddLayer(_T("testtank.xml/MainHP"));
    wndHP->SetAnchor(0.5f, 0.5f);

//     CWindow *wndShopPanel = CWindowManager::GetInstance()->Create<CWindow>(_T("backpack"), wndBG);
//     wndShopPanel->SetPosSize(250.f, 320.f, 0.f, 0.f, 400.f, 400.f, 0.f, 0.f);
//     wndShopPanel->SetAnchor(0.5f, 0.5f);
//     wndShopPanel->SetRenderer(new CBaseRenderer());
//     wndShopPanel->Renderer()->AddLayer(_T("TestGUI/backpack"));
// 
//     CComboBox *combo = CWindowManager::GetInstance()->Create<CComboBox>(_T("bg"), wndShopPanel);
//     combo->SetArea(150.f, 10.f, 0.f, 0.f, 250.f, 40.f, 0.f, 0.f);
//     combo->AddString(_T("Heiti8"));
//     combo->AddString(_T("SLIDER"));
//     combo->AddString(_T("CHECKBOX"));
//     combo->AddString( _T("PROGRESS"));
//     combo->AddString( _T("EDITBOX"));
//     combo->AddString( _T("TEXTLABEL"));
//     combo->AddString( _T("LISTBOX"));
//     combo->AddString( _T("LISTCONTROL"));
//     combo->SubscribeEvent(CComboBox::eET_EVENT_SELECTOK, OnSelChange);
// 
//     CComboBoxRenderer *comboRender = static_cast<CComboBoxRenderer*>(combo->Renderer());
//     comboRender->AddDropDownLayer(_T("ButtonNormal"),CButton::eST_STATE_NORMAL);
//     comboRender->AddComboBoxLayer( _T("SliderBar"), _T("ButtonDisabled"), _T("ButtonNormal"));

}
static float s_f = 1.0f;
void OnClick(CBaseEvent *event)
{
    using namespace FCGUI;
    WindowEvent *winevent = static_cast<WindowEvent *>(event);

    CButton *_pButton = static_cast<CButton *>(winevent->SourceWindow());
    kmVec2 _testPos = _pButton->RealPos();

    _pButton->SetPos( _testPos.x, _testPos.y+s_f );
}

void OnSelChange(CBaseEvent *event)
{
    using namespace FCGUI;
    WindowEvent *winevent = static_cast<WindowEvent *>(event);

    FCGUI::CComboBox *combo = static_cast<FCGUI::CComboBox *>(winevent->SourceWindow());
    int currSel = combo->GetCurrSel();
    CWindow *wndShopPanel = combo->Parent();
    switch (currSel)
    {
    case 0:
        {
            CButton *wndBtn = CWindowManager::GetInstance()->Create<CButton>(_T("TestButton"), wndShopPanel);
            wndBtn->SetArea(10.f, 10.f, 0.f, 0.f, 110.f, 40.f, 0.f, 0.f);
            CBaseRenderer *rdrBtn = static_cast<CBaseRenderer*>(wndBtn->Renderer());
            rdrBtn->AddLayer(_T("ButtonNormal"), CButton::eST_STATE_NORMAL);
            rdrBtn->AddLayer(_T("ButtonPressed"), CButton::eST_STATE_PRESSED);
            rdrBtn->AddLayer(_T("ButtonDisabled"), CButton::eST_STATE_DISABLED);
        }
        break;
    case  1:
        {
            CSlider *slider = CWindowManager::GetInstance()->Create<CSlider>(_T("TestSliser"),wndShopPanel);
            slider->SetArea(10.0f, 80.0f, 0.0f, 0.0f, 120.0f, 91.0f, 0.0f, 0.0f);
            CSliderRenderer* sliderrender = static_cast<CSliderRenderer*>(slider->Renderer());
            sliderrender->AddLayer(_T("SliderBall"),_T("SliderBar"),CSlider::eST_STATE_NORMAL);
        }
        break;
    case  2:
        {
            CCheckBox *checBox = CWindowManager::GetInstance()->Create<CCheckBox>(_T("TestCheckBox1"),wndShopPanel);
            checBox->SetArea(10.0f, 140.0f, 0.f,  0.f, 50.f, 180.f, 0.f, 0.f);

            CCheckBoxRenderer *renderCheckBox = static_cast<CCheckBoxRenderer*>(checBox->Renderer());
            renderCheckBox->AddLayer(_T("CheckBoxBG"),_T("CheckBoxIndicator"),CCheckBox::eST_STATE_NORMAL_UNCHECKED);
            renderCheckBox->AddLayer(_T("CheckBoxBG"),_T("CheckBoxIndicatorChecked"),CCheckBox::eST_STATE_NORMAL_CHECKED);
        }
        break;
    case 3:
        {
            CProgress *progress = CWindowManager::GetInstance()->Create<CProgress>(_T("process"), wndShopPanel);
            progress->SetArea(10.0f,240.0f,0.0f, 0.0f, 210.0f, 260.0f, 0.f, 0.f);

            progress->SetPos(70);
            CProgressRenderer* progressRender = static_cast<CProgressRenderer*>(progress->Renderer());
            progressRender->AddLayer(_T("SliderBall"), _T("SliderBar"), CProgress::eST_STATE_NORMAL);
        }
        break;
    case 4:
        {
            CTextEdit *textedit = CWindowManager::GetInstance()->Create<CTextEdit>(_T("TextEdit"), wndShopPanel);
            textedit->SetArea(0.0f, 320.0f,0.0f, 0.0f, 200.0f, 380.0f, 0.f, 0.f);
            textedit->SetText(_T("±à¼­¿ò"));

            CTextEditRenderer* editrender = static_cast<CTextEditRenderer*>(textedit->Renderer());
            editrender->AddLayer(_T("EditBoxBK"));
        }
        break;
    case  5:
        {
            CTextLabel* label = CWindowManager::GetInstance()->Create<CTextLabel>(_T("TextLabel"), wndShopPanel);
            label->SetArea(300.0f, 250.0f, 0.0f, 0.0f, 380.0f, 300.0f, 0.f, 0.f);
            label->SetText(_T("This is a Text Label"));
        }
        break;
    case  6:
        {
            CListBox *listbox = CWindowManager::GetInstance()->Create<CListBox>(_T("ListBox"), wndShopPanel);
            listbox->SetArea(250.f, 280.f , 0.f, 0.f, 350.f, 380.f, 0.f, 0.f);
            listbox->AddString(_T("SEL1"));
            listbox->AddString(_T("SEL2"));
            listbox->AddString(_T("SEL3"));
            listbox->AddString(_T("SEL4"));
            listbox->AddString(_T("SEL5"));
            listbox->SetCurrSel(0);
            CListBoxRenderer *listboxRenderer = static_cast<CListBoxRenderer*>(listbox->Renderer());
            listboxRenderer->AddLayer(_T("ButtonDisabled"), _T("ButtonNormal"));
        }
        break;
    case 7:
        {
            CListControl* listctl = CWindowManager::GetInstance()->Create<CListControl>(_T("ListControl"), wndShopPanel);
            listctl->SetArea(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
            listctl->SetColWidth(350.f);
            listctl->SetRowHeight(350.f);
            listctl->Renderer()->AddLayer(_T("EditBoxBK"));
            CWindow *child = CWindowManager::GetInstance()->Create<CWindow>(_T("child0"), listctl);
            listctl->AddItem(child);
            child->Renderer()->AddLayer(_T("EditBoxBK"));

            CWindow *child1 = CWindowManager::GetInstance()->Create<CWindow>(_T("child1"), listctl);
            listctl->AddItem(child1);
            child1->Renderer()->AddLayer( _T("SliderBar"));

            CWindow *child2 = CWindowManager::GetInstance()->Create<CWindow>(_T("child2"), listctl);
            listctl->AddItem(child2);
            child2->Renderer()->AddLayer(_T("ButtonDisabled"));

            CWindow *child3 = CWindowManager::GetInstance()->Create<CWindow>(_T("child3"), listctl);
            listctl->AddItem(child3);
            child3->Renderer()->AddLayer(_T("ButtonNormal"));
        }
        break;
    default:
        break;
    }
}
#endif

