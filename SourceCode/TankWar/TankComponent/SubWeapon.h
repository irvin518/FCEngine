#ifndef SUBWEAPON_H_INCLUDE
#define SUBWEAPON_H_INCLUDE
#include "TankComponentBase.h"

class CSubWeapon : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CSubWeapon, 0xCE86AB62, CTankComponentBase);
public:
    CSubWeapon();
    virtual ~CSubWeapon();
    virtual void Update(float fDeltaTime);
    virtual void  ReflectData(CSerializer& serializer) override;
private:
    size_t m_uBulletMaxCount;
};

class CSubWeaponConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CSubWeaponConfigs, 0xCE86AB66, CComponentBase);
public:
    CSubWeaponConfigs();
    virtual ~CSubWeaponConfigs();
    virtual void  ReflectData(CSerializer& serializer) override;
private:
    std::vector<CSubWeapon*>  m_vSubWeaponList;
};

#endif//SUBWEAPON_H_INCLUDE
