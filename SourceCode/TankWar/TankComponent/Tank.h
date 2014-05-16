#ifndef TANK_H__INCLUDE
#define TANK_H__INCLUDE
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "TankWar/Control/AuxiliaryAim.h"
#include "math/kazmath/kazmath/vec2.h"

class CPedesta;
class CTurret;
class CFireController;
class CShield;

class CTank : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CTank, 0x3F8C5B06, CComponentBase)
public:
    enum ETank_State
    {
        eTS_TANKSTATE_NORMAL,
        eTS_TANKSTATE_DIE,
    };

    CTank();
    virtual ~CTank();

    virtual void ReflectData(CSerializer& serializer) override;
    void Update(float fDeltaTime);

    CTurret* GetTurret() const;
    CPedesta* GetPedesta() const;

    void Hurt(kmScalar fHurtData);
    void Die();
    bool IsDie();
    void Display();
    void Hide();
    void UseItem(int iItemId);
    void Recover(kmScalar fRecoverHP);
private:
    CPedesta* m_pPedesta;
    CTurret* m_pTurret;
    CFireController* m_pFireController;
    CShield* m_pShield;
    kmVec2 m_vPosition;
    kmScalar m_fDirection;
    kmScalar m_fHP;
    ETank_State m_eTankState;
};

class CTankConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CTankConfigs, 0x3F8C5B08, CComponentBase);
public:
    CTankConfigs();
    virtual ~CTankConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CTank*> m_vTankList;
};

#endif //TANK_H__INCLUDE