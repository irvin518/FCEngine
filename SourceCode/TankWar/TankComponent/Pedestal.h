#ifndef PEDESTAL_H_INCLUDE
#define PEDESTAL_H_INCLUDE
#include "TankComponentBase.h"

class CPedesta : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CPedesta, 0xC612B37F, CTankComponentBase);
public:
    CPedesta();
    virtual ~CPedesta();

    virtual void ReflectData(CSerializer& serializer) override;
    virtual void Update(float fDeltaTime);

    void Move(kmVec2 direction);
    void Stop();
private:
    kmScalar m_fMaxMoveSpeed;
    kmScalar m_fFaccelerated;
    kmScalar m_fAngularSpeed;

    bool m_bMove;
    kmVec2 m_targeyDirection;
    kmVec2 m_currDirection;
};


class CPedestaConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CPedestaConfigs, 0xC622B37F, CComponentBase);
public:
    CPedestaConfigs();
    virtual ~CPedestaConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CPedesta*> m_vPedestaList;    
};
#endif
