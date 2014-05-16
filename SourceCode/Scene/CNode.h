#ifndef NODE_H_INCLUDE
#define NODE_H_INCLUDE

#include "Render/RenderObject.h"

class  CNode;

typedef std::map< TString, CNode* > TNodeMap;
typedef TNodeMap::const_iterator TNodeMapConstIter;
typedef std::vector< CNode* > TNodeVector;


class CNode
{
public:
    CNode( const TString& name );
    virtual ~CNode();

    void SetVisible( bool bVisible );
    bool IsVisible();

    void AddChild( CNode* pChild );
    CNode* GetChildByName( const TString& name );

    const TNodeMap& GetChildren();

    bool RemoveChild( CNode* pNode );
    const TString& GetName();
    CNode* GetParent();

    virtual void AttachRenderObject( CRenderObject* pRenderObject );
    virtual void DettachRenderObject();
    CRenderObject* GetRenderObject();

private:

    void SetParent( CNode* pParent );

private:

    TNodeMap m_childMap;
    CNode* m_pParentNode;
    TString m_strName;
    bool m_bVisible;
    CRenderObject* m_pRenderObject;
};

#endif//NODE_H_INCLUDE