#ifndef DROPITEM_H_INCLUDE
#define DROPITEM_H_INCLUDE
#include "ActionBase.h"

class CDropItem : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CDropItem, 0xAD3A866B, CActionBase)
public:
    CDropItem();
    virtual ~CDropItem();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(CTank& tank);

};

#endif//!DROPITEM_H_INCLUDE