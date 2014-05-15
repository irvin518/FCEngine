#include "stdafx.h"
#include "TextureFragDescription.h"
#include "TextureAtlas.h"

CTextureFragDescription::CTextureFragDescription()
    : m_pFrag(nullptr)
{

}

CTextureFragDescription::CTextureFragDescription(SharePtr<CTextureAtlas> pAtlas, const TString &strFragName)
    : m_pAtlas(pAtlas)
    , m_strFragName(strFragName)
    , m_pFrag(pAtlas->GetTextureFrag(strFragName))
{

}

CTextureFrag *CTextureFragDescription::GetTextureFrag() const
{
    return m_pFrag;
}

void CTextureFragDescription::ReflectData( CSerializer& serializer )
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_pAtlas, true, 0xFFFFFFFF, _T("纹理集"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strFragName, true, 0xFFFFFFFF, _T("子纹理名"), NULL, NULL, NULL);
}

bool CTextureFragDescription::OnPropertyChange( void* pVariableAddr, void* pNewValueToBeSet )
{
    bool bHandled = super::OnPropertyChange(pVariableAddr, pNewValueToBeSet);
    if(!bHandled)
    {
        if(pVariableAddr == &m_pAtlas)
        {
            DeserializeVarialble(m_pAtlas, (CSerializer*)pNewValueToBeSet);
            if (m_pAtlas && !m_strFragName.empty())
            {
                m_pFrag = m_pAtlas->GetTextureFrag(m_strFragName);
            }
            bHandled = true;
        }
        else if(pVariableAddr == &m_strFragName)
        {
            DeserializeVarialble(m_strFragName, (CSerializer*)pNewValueToBeSet);
            if (m_pAtlas && !m_strFragName.empty())
            {
                m_pFrag = m_pAtlas->GetTextureFrag(m_strFragName);
            }
            bHandled = true;
        }
    }
    return bHandled;
}
