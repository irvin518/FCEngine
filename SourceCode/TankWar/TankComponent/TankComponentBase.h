#ifndef TANKCOMPONENT_H__INCLUDE
#define TANKCOMPONENT_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CTankComponentBase : public CComponentBase
{
    DECLARE_REFLECT_GUID(CTankComponentBase, 0x15103DF3, CComponentBase)
public:
    CTankComponentBase();
    virtual ~CTankComponentBase();
    virtual void ReflectData(CSerializer& serializer) override;
    virtual void Update(float fDeltaTime);
};

#endif //TANKCOMPONENT_H__INCLUDE