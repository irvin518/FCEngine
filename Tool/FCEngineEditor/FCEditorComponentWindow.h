#ifndef FCENGINEEDITOR_FCEDITORCOMPONENTWINDOW_H__INCLUDE
#define FCENGINEEDITOR_FCEDITORCOMPONENTWINDOW_H__INCLUDE

#include "wx/glcanvas.h"
#include "FCEditorGLWindow.h"

class CCamera;
class CShaderProgram;
class CComponentEditorProxy;
class CDependencyDescription;
class CDependencyDescriptionLine;
class CEditorMainFrame;
class CFCEditorGLWindow;

class CFCEditorComponentWindow : public CFCEditorGLWindow
{
public:
    CFCEditorComponentWindow(wxWindow *parent,
        wxGLContext* pShareContext = NULL,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("FCEditorGLWindow"));

    virtual ~CFCEditorComponentWindow();
    wxGLContext* GetGLContext() const;
    void SetGLContext(wxGLContext* pContext);

    void UpdateAllDependencyLine();
    void DeleteSelectedComponent();
    void DeleteSelectedDependencyLine();

    void SetDraggingFileName(const TCHAR* pFileName);
    const TCHAR* GetDraggingFileName();
    void SetDraggingComponent(CComponentEditorProxy* pDraggingComponent);
    CComponentEditorProxy* GetDraggingComponent();
    CCamera* GetCamera() const;
    void ConvertGridPosToWorldPos( int gridX, int gridY, float* pOutWorldPosX, float* pOutWorldPosY);

protected:
    virtual void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouseMidScroll(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseRightUp(wxMouseEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnComponentMenuClicked(wxMenuEvent& event);
    void OnDependencyMenuClicked(wxMenuEvent& event);
    void OnKeyDown(wxKeyEvent& aEvent);

private:
    void ResetProjectionMode();
    void Render();
    void RenderGridLine();
    void RenderComponents();
    void RenderDraggingDependencyLine();

    void ConvertWorldPosToGridPos(const kmVec2* pVectorPos, int* pOutGridX, int* pOutGridY);
    void ConvertWindowPosToWorldPos(const wxPoint& windowPos, float* pOutWorldPosX, float* pOutWorldPosY);
    CComponentEditorProxy* HitTestForComponent( wxPoint pos, enum EComponentAeraRectType* pOutAreaType = NULL, void** pReturnData = NULL);


private:

    int             m_iWidth;
    int             m_iHeight;
    int             m_pOutX;
    int             m_pOutY;
    kmVec2          m_startDragPos;
    float           m_fCellSize;
    TString         m_draggingFilePath;

    wxGLContext*    m_glRC;
    CCamera*        m_pCamera;
    wxMenu*         m_pComponentMenu; 
    wxMenu*         m_pAddDependencyMenu;
    CShaderProgram* m_pLineProgram;
    CEditorMainFrame*       m_pMainFrame;
    CDependencyDescription* m_pAutoSetDependency;
    CComponentEditorProxy*  m_pDraggingComponent;
    CComponentEditorProxy*  m_pConnectComponent;
    CComponentEditorProxy*  m_pCopyComponent;
    CComponentEditorProxy*  m_pClickedComponent;
    CDependencyDescription* m_pDraggingDependency;
    CDependencyDescriptionLine* m_pSelectedDependencyLine;

    std::map<wxMenuItem*, size_t> m_menuItemAndGuidMap;
    wxDECLARE_NO_COPY_CLASS(CFCEditorComponentWindow);
    DECLARE_EVENT_TABLE()
};
#endif