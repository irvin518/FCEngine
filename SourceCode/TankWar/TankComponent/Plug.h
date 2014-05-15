#ifndef PLUG_H_INCLUDE
#define PLUG_H_INCLUDE
#include "TankComponentBase.h"
#include "PlugComponentBase.h"

class CPlug : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CPlug, 0x674ABAA0, CTankComponentBase)
public:
    CPlug();
    virtual ~CPlug();
    virtual void ReflectData(CSerializer& serializer) override;

    virtual void Updata();
private:
    std::vector<CPlugComponentBase*> m_plugList;
};

class CPlugConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CPlugConfigs, 0x674ABAA1, CComponentBase)
public:
    CPlugConfigs();
    virtual ~CPlugConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CPlug*> m_vPlugList;
};
#endif//PLUG_H_INCLUDE
