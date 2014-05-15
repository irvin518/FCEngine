#ifndef TURRET_H_INCLUDE
#define TURRET_H_INCLUDE
#include "TankComponentBase.h"
class CMainWeapon;
class CSubWeapon;
class CTurret : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CTurret, 0xC727BAC2, CTankComponentBase);
public:
    enum EWeapon_Type
    {
        eWT_MAINWEAPON,
        eWT_SUBWEAPON,
    };
public:
    CTurret();
    virtual ~CTurret();
    virtual void ReflectData(CSerializer& serializer) override;

    void Update(float fDeltaTime);
    void Shot();
private:
    bool IsCanShot();
private:
    CMainWeapon* m_pMainWeapon;
    CSubWeapon* m_pSubWeapon;

    kmScalar m_fFireInterval;
    kmScalar m_fAngleSpeed;
    size_t m_uSingleBulletCount;
    size_t m_uLoaddingSpeed;

    EWeapon_Type m_eActiveWeaponType;

    unsigned long m_lLastShotTime;

};

class CTurretConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CTurretConfigs, 0xC727BAC8, CComponentBase);
public:
    CTurretConfigs();
    virtual ~CTurretConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CTurret*> m_vTurretList;
};

#endif//TURRET_H_INCLUDE
