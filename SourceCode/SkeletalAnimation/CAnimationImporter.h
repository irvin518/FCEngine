#ifndef  SKELETALANIMATION_CIMPORTER_H__INCLUDE
#define SKELETALANIMATION_CIMPORTER_H__INCLUDE

struct Json;
namespace _2DSkeletalAnimation
{
    class CBoneData;
    class CMoveData;
    struct SMoveBoneData;
    struct SDisplayData;
    class CArmatureData;
    class CAnimationData;
    class CAnimationImporter
    {
        BEATS_DECLARE_SINGLETON(CAnimationImporter)

    public:
        void ImportData(const TString& file);

    private:
        void DecodeArmature(Json* json);
        void DecodeBoneData(Json* json, CArmatureData* pArmatureData);
        void DecodeDisplayData(Json* json, CBoneData* pBoneData);
        void DecodeSkinData(Json* json, SDisplayData* pDisplayData);

        void DecodeAnimation(Json* json);
        void DecodeMovData(Json* json, CAnimationData* pAnimationData);
        void DecodeMovBoneData(Json *json, CMoveData* pMoveData);
        void DecodeFrameData(Json* json, SMoveBoneData* pMoveBoneData);

        void DecodeTexture(Json* json);

        const char* GetStringFromFile(const char* file);
    };
}
#endif//SKELETALANIMATION_CDATAIMPORTER_H__INCLUDE