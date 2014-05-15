#ifndef BULLET_H__INCLUDE
#define BULLET_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CBullet : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CBullet, 0xAF815B06, CComponentBase)

public:
    CBullet();

    virtual ~CBullet();
    virtual void ReflectData(CSerializer& serializer) override;

private:
	float m_fSpeed;
	float m_fMaxSpeed;
	float m_fMaxDistance;
	float m_fAcceleration;
	unsigned int  m_uDamage;
	int m_nDamageFloat;
	int m_nDamageScope;
	bool m_bIsTrace;
	unsigned int m_uSteeringAngularVelocity;
	int m_nBulletSkillID;
};

class CBulletConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CBulletConfigs, 0xAF815B08, CComponentBase)
public:
    CBulletConfigs();
    virtual ~CBulletConfigs();
    virtual void ReflectData(CSerializer& serializer) override;

private:
    std::vector<CBullet*>  m_vBulletList;
};

#endif //BULLET_H__INCLUDE