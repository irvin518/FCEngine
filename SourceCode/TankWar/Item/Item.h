#ifndef ITEM_H__INCLUDE
#define ITEM_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CItem : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CItem, 0xAA215B02, CComponentBase)
public:
    CItem();
    virtual ~CItem();
    virtual void ReflectData(CSerializer& serializer) override;
private:
};

class CItemConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CItemConfigs, 0xAA215B08, CComponentBase)

public:
    CItemConfigs();
    virtual ~CItemConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CItem*> m_vItemList;
};

#endif //ITEM_H__INCLUDE