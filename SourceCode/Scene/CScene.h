#ifndef SCENE_H_INCLUDE
#define SCENE_H_INCLUDE
#include "Camera.h"
#include "CLayer.h"

class CScene
{

public:
    CScene( const TString& name );
    ~CScene();

    CLayer* CreateLayer( );

    void AddLayer( CLayer* pNode );

    void RemoveLayer( CLayer* pNode );

    unsigned int GetLayerNumber( ) const;

    CLayer* GetLayerByName( const TString& name );

    const TNodeVector& GetVisibleNode( CCamera* pCamera );

private:
    TNodeMap m_nodeMap;
    TNodeVector m_nodeVisibleVector;
    TString m_strName;
};

#endif