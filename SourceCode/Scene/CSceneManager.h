#ifndef SCENEMANAGE_H_INCLUDE
#define SCENEMANAGE_H_INCLUDE

#include "CScene.h"

class CSceneManager
{
    typedef std::map< TString, CScene* > TSceneMap;
    BEATS_DECLARE_SINGLETON( CSceneManager );
public:

    bool AddScene( CScene* pScene );

    void RemoveScene( CScene* pScene );

    unsigned int GetSceneNumber();

    CScene* GetSceneByName( const TString& name ) const;

    CScene* GetCurrentScene();
    void SetCurrentScene( CScene* pScene );
private:

    CScene* m_pCurrentScene;
    TSceneMap m_allSceneMap;

};

#endif