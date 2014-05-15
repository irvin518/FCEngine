#ifndef RENDER_SKELETON_H__INCLUDE
#define RENDER_SKELETON_H__INCLUDE

#include "resource/Resource.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
class CSkeletonBone;

class CSkeleton : public CResource
{
    typedef CResource super;
    DECLARE_REFLECT_GUID(CSkeleton, 0x1147A0EF, CResource)
    DECLARE_RESOURCE_TYPE(eRT_Skeleton)

public:
    CSkeleton();
    virtual ~CSkeleton();

    virtual void ReflectData(CSerializer& serializer) override;
    virtual bool Load();
    virtual bool Unload();

    SharePtr<CSkeletonBone> GetSkeletonBone(ESkeletonBoneType type) const;
    std::map<ESkeletonBoneType, SharePtr<CSkeletonBone>>& GetBoneMap();
private:
    std::map<ESkeletonBoneType, SharePtr<CSkeletonBone>> m_bonesMap;
};

#endif