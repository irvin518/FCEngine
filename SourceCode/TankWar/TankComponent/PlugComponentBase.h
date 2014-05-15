#ifndef PLUGCOMPONENTBASE_INCLUDE
#define  PLUGCOMPONENTBASE_INCLUDE
#include "TankComponentBase.h"

class CPlugComponentBase : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CPlugComponentBase, 0x674C1CAB, CTankComponentBase)
public:
    CPlugComponentBase();
    virtual ~CPlugComponentBase();
    virtual void ReflectData(CSerializer& serializer) override;
};

#endif//PLUGCOMPONENTBASE_INCLUDE
