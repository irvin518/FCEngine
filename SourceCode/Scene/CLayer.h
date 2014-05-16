#ifndef LAYER_H_INCLUDE
#define LAYER_H_INCLUDE

#include "CNode2D.h"

class CLayer : public CNode2D
{
public:
    CLayer( const TString& name );
    virtual ~CLayer();

    void AddNode( CNode2D* pNode );
    bool RemoveNode( CNode2D* pNode );
    void RemoveNodeByName( const TString& name );

    CNode2D* GetNodeByName( const TString& name );
    const TNodeVector& GetNodes() const;

    void OnTouchBegin();
    void OnTouchEnd();
    void OnTouchMove();
    void OnTOuchCanle();

private:
    TNodeVector m_nodeVector;
};
#endif

