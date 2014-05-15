#ifndef  SKELETALANIMATION_CANIMATIONDATA_H__INCLUDE
#define SKELETALANIMATION_CANIMATIONDATA_H__INCLUDE

namespace _2DSkeletalAnimation
{
    class CMoveData;
    class  CAnimationData 
    {
        typedef std::map<std::string, CMoveData*> MoveDataMap;
    public:
        CAnimationData();
        ~CAnimationData();
        void AddMoveData(CMoveData *moveData);
        CMoveData *GetMovData(const std::string& moveName);
        void SetName(const std::string name);
        const std::string& GetName() const;
        const std::vector<std::string>& GetMoveNameList();
    private:
        std::string m_strName;
        MoveDataMap m_movedataMap;
        std::vector<std::string> m_moveNameList;
    };
}

#endif//SKELETALANIMATION_CANIMATIONDATA_H__INCLUDE