#ifndef  SKELETALANIMATION_CARMATURE_H__INCLUDE
#define SKELETALANIMATION_CARMATURE_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

namespace _2DSkeletalAnimation
{
    class CAnimationRenderer;
    class CAnimationController;
    struct SDisplayData;
    class CAnimationModel : public CComponentBase
    {
        typedef CComponentBase super;
        DECLARE_REFLECT_GUID(CAnimationModel, 0xB4ADE69A, CComponentBase)
    public:
        CAnimationModel();
        ~CAnimationModel();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual bool OnPropertyChange(void* pVariableAddr, void* pNewValueToBeSet) override;

        void Load();

        void RenderSkeletal();
        void Update();
        void ReplaceDisplayData(const std::string& boneName, const SDisplayData& displayData);
    private:
        void Init();
    private:
        SReflectFilePath m_strCfgFilePath;
        TString m_strResourcePath;
        TString m_strMoveName;

        CAnimationController* m_pAnimationController;
        CAnimationRenderer* m_pAnimationRenderer;

        std::vector<TString> m_moveNameList;
        bool m_bPlayAnimation;
    };
}

#endif//SKELETALANIMATION_CARMATURE_H__INCLUDE
