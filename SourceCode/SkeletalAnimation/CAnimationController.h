#ifndef  SKELETALANIMATION_CANIMATIONCONTROLLER_H__INCLUDE
#define SKELETALANIMATION_CANIMATIONCONTROLLER_H__INCLUDE

namespace _2DSkeletalAnimation
{
    class CMoveData;
    class CArmatureData;
    struct SFrameData;
    class CAnimationModel;
    class CAnimationController 
    {
        typedef std::map<std::string, SFrameData> TFrameDataMap;
    public:
        CAnimationController(CAnimationModel* pModel);
        ~CAnimationController();
        const std::string& GetAnimationName() const;
        const std::string& GetArmatureName() const;
        void SetAnimationName(const std::string& name);
        void SetArmatureName(const std::string& name);
        void PlayMove(CMoveData* pMoveData);
        void PlayMove(const TString& moveName);
        const CMoveData* GetCurrMove() const;
        TFrameDataMap& GetInitBonesData();
        TFrameDataMap& GetCurrFrameBonesData();
        const std::vector<std::string>& GetBoneNames();
        size_t GetCurrFrame();

        void Play();
        void Update(float fDeltaTime);
        void GoToFrame(size_t frame);
        void Pause();
        void Resume();
        void Stop();
        bool IsPlaying();

    private:
        void InsertFrameData(SFrameData& result,  const SFrameData& beforeData, const SFrameData& afterData, int iFrame);
        bool IsBlendState();
        void GetMoveFrameData(TFrameDataMap& frameDataMap, CMoveData* pMoveData);
        void GetFrameDataByFrame(TFrameDataMap& frameDataMap, CMoveData* pMoveData, int iFrame);
        void Interpolation(const TFrameDataMap& preFrameDataMap, const TFrameDataMap& currFrameDataMap);

    private:
        CAnimationModel* m_pModel;

        CMoveData* m_pPreMove;
        CMoveData* m_pCurrMove;
        TFrameDataMap m_frameDataMap;

        std::vector<std::string> m_boneNames;
        bool m_bLoop;
        bool m_bHaveRender;
        size_t m_uLoopCount;
        bool m_bPlaying;
        float m_fPlayingTime;
        float m_fBlendTime;
        float m_fDuration;
        size_t m_uFrameCount;
        size_t m_uFps;

        std::string m_strAnimationName;
        std::string m_strArmatureName;
    };
}
#endif//SKELETALANIMATION_CANIMATIONCONTROLLER_H__INCLUDE
