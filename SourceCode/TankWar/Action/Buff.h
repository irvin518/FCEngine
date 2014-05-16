#ifndef TANKWAR_ACTION_BUFF_H_INCLUDE
#define TANKWAR_ACTION_BUFF_H_INCLUDE

#include "ActionBase.h"

class CBuff : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CBuff, 0x2341541D, CActionBase)
public:
    CBuff();
    virtual ~CBuff();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);
};

#endif
