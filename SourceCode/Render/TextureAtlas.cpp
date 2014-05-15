#include "stdafx.h"
#include "TextureAtlas.h"
#include "Utility/TinyXML/tinyxml.h"
#include "Utility/BeatsUtility/StringHelper.h"
#include "Resource/Resource.h"
#include "Resource/ResourceManager.h"
#include "TextureFrag.h"
#include "Texture.h"
#include "TextureFragManager.h"

CTextureAtlas::CTextureAtlas()
{

}

CTextureAtlas::~CTextureAtlas()
{
    Unload();
}

bool CTextureAtlas::Load()
{
    BEATS_ASSERT(!IsLoaded());

    TString fileext = GetFilePath().substr(GetFilePath().size() - 3);
    if(fileext == _T("xml"))
    {
        char *filepath = new char[GetFilePath().size() + 1];
        CStringHelper::GetInstance()->ConvertToCHAR(GetFilePath().c_str(), filepath, GetFilePath().size() + 1);
        TiXmlDocument doc;
        doc.LoadFile(filepath);
        BEATS_SAFE_DELETE_ARRAY(filepath);

        TiXmlElement *root = doc.RootElement();
        BEATS_ASSERT(strcmp(root->Value(), "Imageset") == 0);
        const char *name = root->Attribute("Name");
        BEATS_ASSERT(name);
        size_t len = strlen(name);
        TCHAR *nameT = new TCHAR[len + 1];
        CStringHelper::GetInstance()->ConvertToTCHAR(name, nameT, len + 1);
        m_name = nameT;
        BEATS_SAFE_DELETE_ARRAY(nameT);

        const char *textureFile = root->Attribute("Imagefile");
        BEATS_ASSERT(textureFile);
        len = strlen(textureFile);
        TCHAR *textureFileT = new TCHAR[len + 1];
        CStringHelper::GetInstance()->ConvertToTCHAR(textureFile, textureFileT, len + 1);
        m_texture = CResourceManager::GetInstance()->GetResource<CTexture>(textureFileT, false);
        BEATS_SAFE_DELETE_ARRAY(textureFileT);

        for(TiXmlElement *elemImage = root->FirstChildElement("Image");
            elemImage != nullptr; elemImage = elemImage->NextSiblingElement("Image"))
        {
            const char *name = elemImage->Attribute("Name");
            BEATS_ASSERT(name);
            kmVec2 point;
            kmVec2Fill(&point, 0.f, 0.f);
            kmVec2 size;
            kmVec2Fill(&size, 0.f, 0.f);
            elemImage->QueryFloatAttribute("XPos", &point.x);
            elemImage->QueryFloatAttribute("YPos", &point.y);
            elemImage->QueryFloatAttribute("Width", &size.x);
            elemImage->QueryFloatAttribute("Height", &size.y);

            size_t len = strlen(name);
            TCHAR *nameT = new TCHAR[len + 1];
            CStringHelper::GetInstance()->ConvertToTCHAR(name, nameT, len + 1);
            CreateTextureFrag(nameT, point, size);
            BEATS_SAFE_DELETE_ARRAY(nameT);
        }
    }
    else
    {
        m_texture = CResourceManager::GetInstance()->GetResource<CTexture>(GetFilePath(), false);
        m_name = GetFilePath();
    }

    CTextureFragManager::GetInstance()->AddTextureAtlas(this);

    SetLoadedFlag(true);

    return true;
}

bool CTextureAtlas::Unload()
{
    BEATS_ASSERT(IsLoaded());

    for(auto frag : m_textureFrags)
    {
        BEATS_SAFE_DELETE(frag.second);
    }

    CTextureFragManager::GetInstance()->RemoveTextureAtlas(m_name);
    SetLoadedFlag(false);
    return true;
}

const TString &CTextureAtlas::Name() const
{
    return m_name;
}

SharePtr<CTexture> CTextureAtlas::Texture() const
{
    return m_texture;
}

CTextureFrag *CTextureAtlas::GetTextureFrag(const TString &name) const
{
    auto itr = m_textureFrags.find(name);
    return itr != m_textureFrags.end() ? itr->second : nullptr;
}

CTextureFrag *CTextureAtlas::CreateTextureFrag(const TString &name, kmVec2 point, kmVec2 size)
{
    BEATS_ASSERT(m_textureFrags.find(name) == m_textureFrags.end());
    CTextureFrag *frag = new CTextureFrag(name, m_texture, point, size);
    m_textureFrags[name] = frag;
    return frag;
}
