#ifndef  SKELETALANIMATION_CARMATUREDATA_H__INCLUDE
#define SKELETALANIMATION_CARMATUREDATA_H__INCLUDE
namespace _2DSkeletalAnimation
{
    class CBoneData;
    struct SDisplayData;
    class CArmatureData
    {
        typedef std::map<std::string, CBoneData*> BonesDataMap;
    public:
        CArmatureData();
        ~CArmatureData();

        void AddBoneData(CBoneData *boneData);
        const CBoneData *GetBoneData(const std::string &boneName) const;
        const std::string& GetName() const;
        void SetName(const std::string& name);
        const BonesDataMap& GetBonesMap() const;
        const std::vector<CBoneData*>& GetBoneOrderList();
        SDisplayData ReplaceDisplayData(const std::string boneName, const SDisplayData& displayData);
    private:
        std::string m_strName;
        BonesDataMap m_boneDataMap;
        std::vector<CBoneData*> m_boneDataOrderList;
    };
}

#endif//SKELETALANIMATION_CARMATUREDATA_H__INCLUDE
