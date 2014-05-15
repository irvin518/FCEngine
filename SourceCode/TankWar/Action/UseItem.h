#ifndef USERITEM_H_INCLUDE
#define USERITEM_H_INCLUDE
#include "ActionBase.h"

class CItem;
class CUseItem : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CUseItem, 0x7AF86A35, CActionBase)
public:
    CUseItem();
    virtual ~CUseItem();

    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(CTank& tank);

private:
    CItem* m_pItem;
};

#endif//!USERITEM_H_INCLUDE