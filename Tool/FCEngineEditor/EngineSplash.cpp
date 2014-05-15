#include "stdafx.h"
#include "EngineSplash.h"
#include "EngineCenter.h"
#include "EditorMainFrame.h"
#include "TransparentText.h"

BEGIN_EVENT_TABLE(CEngineSplash, wxSplashScreen)
    EVT_CLOSE(CEngineSplash::OnCloseWindow)
END_EVENT_TABLE()

CEngineSplash::CEngineSplash(wxBitmap& bitmap, long splashStyle, int milliseconds,
                           wxWindow* parent, wxWindowID id,
                           const wxPoint& pos ,
                           const wxSize& size ,
                           long style) :
                    wxSplashScreen(bitmap, splashStyle, milliseconds, parent, id, pos, size, style)
{
}

CEngineSplash::~CEngineSplash()
{
}

void CEngineSplash::Instance()
{
    wxWindow *win = GetSplashWindow();
    wxStaticText *text = new CTransparentText( win,
        wxID_EXIT,
        wxT("FCEngineEditor V_0_1"),
        wxPoint(13, 11)
        );
    wxFont font = text->GetFont();
    font.SetPointSize(2*font.GetPointSize());
    text->SetFont(font);
}

void CEngineSplash::OnCloseWindow( wxCloseEvent &/*event*/ )
{
    CEditorMainFrame* pMainFrame = (CEditorMainFrame*)GetParent();
    pMainFrame->Show(true);
    pMainFrame->Thaw();
    CEngineCenter::GetInstance()->Initialize();
    pMainFrame->InitComponentsPage();
    pMainFrame->StartRenderUpdate();
    Destroy();
}
