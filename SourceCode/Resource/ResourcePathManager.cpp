#include "stdafx.h"
#include "ResourcePathManager.h"
#include "external/CFileUnitils.h"

CResourcePathManager* CResourcePathManager::m_pInstance = NULL;

CResourcePathManager::CResourcePathManager()
{
    Init();
}

CResourcePathManager::~CResourcePathManager()
{

}

void CResourcePathManager::Init()
{
    TStringVector searchPaths;
    for (auto i = 0; i < eRPT_Count; i++)
    {
        searchPaths.push_back(pszCocosResourcePathName[i]);
    }
    CFileUtils::GetInstance()->SetSearchPaths(searchPaths);
}

CResourcePathManager::EResourcePathType CResourcePathManager::GetResourcePathType(EResourceType type) const
{
    EResourcePathType ret = eRPT_Count;
    switch (type)
    {
    case eRT_Texture:
        ret = eRPT_Texture;
        break;
    case eRT_TextureAtlas:
        ret = eRPT_Texture;
        break;
    case eRT_Skeleton:
        ret = eRPT_Skeleton;
        break;
    case eRT_Skin:
        ret = eRPT_Skin;
        break;
    case eRT_Material:
        ret = eRPT_Material;
        break;
    case eRT_Shader:
        ret = eRPT_Shader;
        break;
    case eRT_SpriteAnimation:
        ret = eRPT_SpriteAnimation;
        break;
    case eRT_Animation:
        ret = eRPT_Animation;
        break;
    case eRT_ParticleScript:
        ret = eRPT_Particle;
        break;
    case eRT_Font:
        ret = eRPT_Font;
        break;
    case eRT_Language:
        ret = eRPT_Language;
        break;

    default:
        break;
    }
    BEATS_ASSERT(ret != eRPT_Count, _T("Unknown type of resource %d"), type);
    return ret;
}

const TString& CResourcePathManager::GetFullPathForFilename( const TString &filename )
{
    m_strFullPathForFilename = CFileUtils::GetInstance()->FullPathForFilename(filename);
    return m_strFullPathForFilename;
}

const TString& CResourcePathManager::GetResourcePath( CResourcePathManager::EResourcePathType type )
{
    const TStringVector& searchPathArray = CFileUtils::GetInstance()->GetSearchPaths();
    m_strResourcePath = searchPathArray[ type ];
    return m_strResourcePath;
}
