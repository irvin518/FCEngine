#ifndef HURT_H_INCLUDE
#define HURT_H_INCLUDE
#include "ActionBase.h"

class CHurt : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CHurt, 0xDF3AD349, CActionBase)
public:
    CHurt();
    virtual ~CHurt();

    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(CTank& tank);

private:

    kmScalar m_fHurtData;
};

#endif//!HURT_H_INCLUDE