#ifndef MAINWEAPON_H_INCLUDE
#define MAINWEAPON_H_INCLUDE
#include "TankComponentBase.h"
#include "../Skill/Skill.h"

class CMainWeapon : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CMainWeapon, 0xC162AEEF, CTankComponentBase);
public:
    CMainWeapon();
    virtual ~CMainWeapon();
    virtual void Update(float fDeltaTime);
    virtual void ReflectData(CSerializer& serializer) override;
    void Shot();

private:
    CSkill* m_pSkill;
};

class CMainWeaponConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CMainWeaponConfigs, 0xC262AEEF, CComponentBase);
public:
    CMainWeaponConfigs();
    virtual ~CMainWeaponConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CMainWeapon*>  m_vMainWeaponList;
};
#endif//MAINWEAPON_H_INCLUDE
