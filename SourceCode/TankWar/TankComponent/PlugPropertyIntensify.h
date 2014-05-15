#ifndef PLUGPROPERTYINTENTSIFY_INCLUDE
#define PLUGPROPERTYINTENTSIFY_INCLUDE

#include "PlugComponentBase.h"

class CPlugPropertyIntentsify : public CPlugComponentBase
{
    DECLARE_REFLECT_GUID(CPlugPropertyIntentsify, 0x1DEA512B, CPlugComponentBase);
public:
    CPlugPropertyIntentsify();
    virtual ~CPlugPropertyIntentsify();
    //virtual void ReflectData(CSerializer& serializer) override;

private:

};
#endif//PLUGPROPERTYINTENTSIFY_INCLUDE
