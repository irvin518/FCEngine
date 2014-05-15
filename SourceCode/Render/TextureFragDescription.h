#ifndef RENDER_TEXTURE_FRAG_DESCRIPTION_H__INCLUDE
#define RENDER_TEXTURE_FRAG_DESCRIPTION_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CTextureAtlas;
class CTextureFrag;

class CTextureFragDescription : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CTextureFragDescription, 0xB4ADE61F, CComponentBase);
public:
    CTextureFragDescription();
    CTextureFragDescription(SharePtr<CTextureAtlas> pAtlas, const TString &strFragName);

    CTextureFrag *GetTextureFrag() const;

    virtual void ReflectData( CSerializer& serializer ) override;

    virtual bool OnPropertyChange( void* pVariableAddr, void* pNewValueToBeSet ) override;

private:
    SharePtr<CTextureAtlas> m_pAtlas;
    TString m_strFragName;
    CTextureFrag *m_pFrag;
};

#endif 
