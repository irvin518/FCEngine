#ifndef FIRECONTROLLER_H_INCLUDE
#define FIRECONTROLLER_H_INCLUDE
#include "TankComponentBase.h"

class CFireController : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CFireController, 0x0131C1E1, CTankComponentBase);
public:
    CFireController();
    virtual ~CFireController();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    kmScalar m_fSectorRadius;
    kmScalar m_fSectorAngle;
    kmScalar m_fMaxAnglePercent;
    kmScalar m_fMinAngelPercent;
    kmScalar m_fShrinkSpeed;
};


class CFireControllerConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CFireControllerConfigs, 0x0131C1E2, CComponentBase);
public:
    CFireControllerConfigs();
    virtual ~CFireControllerConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CFireController*>  m_vFireControllerList;
};
#endif//FIRECONTROLLER_H_INCLUDE