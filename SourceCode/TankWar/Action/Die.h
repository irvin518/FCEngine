#ifndef DIE_H_INCLUDE
#define DIE_H_INCLUDE
#include "ActionBase.h"

class CDie : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CDie, 0x83A9EB00, CActionBase)
public:
    CDie();
    virtual ~CDie();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(CTank& tank);

};

#endif//!DIE_H_INCLUDE