#ifndef SHOT_H_INCLUDE
#define SHOT_HINCLUDE
#include "ActionBase.h"

class CShot : public CActionBase
{
    DECLARE_REFLECT_GUID(CShot, 0xFAAAB890, CActionBase)
public:
    CShot();
    virtual ~CShot();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);

};

#endif//!SHOT_H_INCLUDE
