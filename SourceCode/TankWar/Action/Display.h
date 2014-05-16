#ifndef TANKWAR_ACTION_DISPLAY_H_INCLUDE
#define TANKWAR_ACTION_DISPLAY_H_INCLUDE
#include "ActionBase.h"

class CDisplay : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CDisplay, 0xEC85C9B1, CActionBase)
public:
    CDisplay();
    virtual ~CDisplay();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);

};

#endif//!TANKWAR_ACTION_DISPLAY_H_INCLUDE
