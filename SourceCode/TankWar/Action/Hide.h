#ifndef TANKWAR_ACTION_HIDE_H_INCLUDE
#define TANKWAR_ACTION_HIDE_H_INCLUDE
#include "ActionBase.h"

class CHide : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CHide, 0xB0BD8DA3, CActionBase)
public:
    CHide();
    virtual ~CHide();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);

};

#endif//!TANKWAR_ACTION_HIDE_H_INCLUDE
