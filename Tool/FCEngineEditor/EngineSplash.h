#ifndef FCENGINEEDITOR_ENGINESPLASH_H__INCLUDE
#define FCENGINEEDITOR_ENGINESPLASH_H__INCLUDE

#include "wx/image.h"
#include "wx/splash.h"
#include "wx/mediactrl.h"

class CEngineSplash : public wxSplashScreen
{
public:
    CEngineSplash(wxBitmap& bitmap, long splashStyle, int milliseconds,
                    wxWindow* parent, wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);
    virtual ~CEngineSplash();
    void Instance();
    void OnCloseWindow(wxCloseEvent &event);
    DECLARE_EVENT_TABLE();
};

#endif