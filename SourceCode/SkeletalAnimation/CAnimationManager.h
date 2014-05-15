#ifndef SKELETALANIMATION_CANIMATIONMANAGER_H__INCLUDE
#define SKELETALANIMATION_CANIMATIONMANAGER_H__INCLUDE

#include "CAnimationData.h"
namespace _2DSkeletalAnimation
{
    class CArmatureData;
    class CTextureData;
    struct SDisplayData;
    class CAnimationController;
    class CAnimationModel;
    class  CAnimationManager
    {
        BEATS_DECLARE_SINGLETON(CAnimationManager)
    public:
        void AddAnimationDataFromFile(const TString &file);
        CAnimationController* CreateController(CAnimationModel* pModel);
        CAnimationData* GetAnimationData(const std::string& name) const;
        CArmatureData* GetArmatureData(const std::string& name) const;
        void AddAnimationData(CAnimationData* pAnimationData);
        void AddArmatureData(CArmatureData* pArmatureData);
        std::vector<CTextureData*>& GetTextureData();
        void ReplaceDisplayData(const std::string& boneName, const SDisplayData& displayData);
        const std::string GetLastAnimationName() const;
        const std::string GetLastArmatureName() const;
        void Update(float fDeltaTime);
        void DeleteController(CAnimationController *controller);
    private:
        std::vector<CTextureData*> m_textureList;
        std::map<std::string, CAnimationData*> m_animationDataMap;
        std::map<std::string, CArmatureData*> m_armatureDataMap;

        std::list<CAnimationController*> m_controllerList;
        std::map<std::string, std::string> m_animationNameMap;

        std::string m_strLastAnimationName;
        std::string m_strLastArmatureName;
    };
}

#endif//SKELETALANIMATION_CANIMATIONMANAGER_H__INCLUDE