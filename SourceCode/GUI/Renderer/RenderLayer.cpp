#include "stdafx.h"
#include "RenderLayer.h"
#include "Render/TextureFrag.h"
#include "Render/TextureFragManager.h"
#include "Render/Material.h"
#include "GUI/System.h"
#include "Render/TextureFragDescription.h"
#include "Render/TextureAtlas.h"
#include "Resource/ResourceManager.h"

using namespace FCGUI;

CRenderLayer::CRenderLayer()
{
    Init();
}

CRenderLayer::CRenderLayer(const TString &textureAtlasName, const TString &textureFragName )
{
    SharePtr<CTextureAtlas> pAtlas = CResourceManager::GetInstance()->
        GetResource<CTextureAtlas>(textureAtlasName, false);
    BEATS_ASSERT(pAtlas);
    m_frames.push_back(new CTextureFragDescription(pAtlas, textureFragName));
    Init();
}

CRenderLayer::CRenderLayer( const TFrameList &frames )
{
    m_frames.assign(frames.begin(), frames.end());
    Init();
}

void CRenderLayer::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_frames, true, 0xFFFFFFFF, _T("Ö¡"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_uCurrFrame, true, 0xFFFFFFFF, _T("µ±Ç°Ö¡"), NULL, NULL, NULL);
}

CRenderLayer::~CRenderLayer()
{
    for(auto frame : m_frames)
    {
        BEATS_SAFE_DELETE(frame);
    }
}

void CRenderLayer::Init()
{
    m_uCurrFrame = 0;
    InitAnimProp();
    m_pMaterial = CSystem::GetInstance()->CreateDefaultMaterial();
    if (m_uCurrFrame < m_frames.size())
        m_pMaterial->SetTexture(0, m_frames[m_uCurrFrame]->GetTextureFrag()->Texture());
}

void CRenderLayer::InitAnimProp()
{
    AddProp(PROP_CURR_FRAME, _T("Current Frame"), 
        [this](kmScalar currFrame){SetCurrFrame((size_t)currFrame);},
        [this](){return (kmScalar)CurrFrame();});
}

void CRenderLayer::SetCurrFrame(size_t currFrame)
{
    if (!m_frames.empty())
    {
        if(currFrame >= m_frames.size())
        {
            m_uCurrFrame = m_frames.size() - 1;
        }
        else
        {
            m_uCurrFrame = currFrame;
        }
        m_pMaterial->SetTexture(0, m_frames[m_uCurrFrame]->GetTextureFrag()->Texture());
    }
}

size_t CRenderLayer::CurrFrame() const
{
    return m_uCurrFrame;
}

CTextureFrag *CRenderLayer::GetTextureFrag() const
{
    CTextureFrag *frag = nullptr;
    if (m_uCurrFrame < m_frames.size())
    {
        frag = m_frames[m_uCurrFrame]->GetTextureFrag();
    }
    return frag;
}

SharePtr<CMaterial> CRenderLayer::GetMaterial() const
{
    return m_pMaterial;
}