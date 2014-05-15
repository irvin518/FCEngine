#ifndef GUI_RENDERER_RENDER_LAYER_H__INCLUDE
#define GUI_RENDERER_RENDER_LAYER_H__INCLUDE

#include "GUI/Animation/Animatable.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CTextureAtlas;
class CTextureFrag;
class CTextureFragDescription;
class CMaterial;

namespace FCGUI
{
    class CRenderLayer : public CAnimatable, public CComponentBase
    {
        typedef CComponentBase super;
        DECLARE_REFLECT_GUID(CRenderLayer, 0xB4ADE61E, CComponentBase);
    public:
        typedef std::vector<CTextureFragDescription *> TFrameList;

        enum EProperty
        {
            PROP_CURR_FRAME,
        };

        CRenderLayer();
        CRenderLayer(const TString &textureAtlasName, const TString &textureFragName);
        CRenderLayer(const TFrameList &frames);
        virtual ~CRenderLayer();
        virtual void ReflectData( CSerializer& serializer ) override;

        void Init();

        void SetCurrFrame(size_t currFrame);
        size_t CurrFrame() const;
        CTextureFrag *GetTextureFrag() const;
        SharePtr<CMaterial> GetMaterial() const;

    private:
        void InitAnimProp();

    private:
        TFrameList m_frames;
        size_t m_uCurrFrame;
        SharePtr<CMaterial> m_pMaterial;
    };
}

#endif // !GUI_RENDERER_RENDER_LAYER_H__INCLUDE
