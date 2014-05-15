#ifndef SHIELD_H_INCLUDE
#define SHIELD_H_INCLUDE
#include "TankComponentBase.h"

class CShield : public CTankComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CShield, 0x623ACC5F, CTankComponentBase);
public:
    CShield();
    virtual ~CShield();

    virtual void ReflectData(CSerializer& serializer) override;
    virtual void Update(float fDeltaTime);

    kmScalar GetHP() const;
    void SetHP(const kmScalar fHP);
    kmScalar Hurt(kmScalar fHurtData);
private:
    kmScalar m_fHP;
    kmScalar m_fHPReSpeed;
};

class CShieldConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CShieldConfigs, 0x626ACC5F, CComponentBase);
public:
    CShieldConfigs();
    virtual ~CShieldConfigs();
     virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CShield* > m_vShieldList;
};
#endif//SHIELD_H_INCLUDE
