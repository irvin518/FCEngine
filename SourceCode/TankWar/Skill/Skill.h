#ifndef SKILL_H__INCLUDE
#define SKILL_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "TankWar/Bullet/Bullet.h"

class CSkill : public CComponentBase
{
    typedef CComponentBase super;
	DECLARE_REFLECT_GUID( CSkill, 0xF044A191, CComponentBase )
public:
	CSkill();
	virtual ~CSkill();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    //
    CBullet* m_pBullet;
    bool m_bWeapon;
};

class CSkillConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID( CSkillConfigs, 0xF044A232, CComponentBase )
public:
    CSkillConfigs();
    virtual ~CSkillConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CSkill*> m_vSkillList;
};

#endif //SKILL_H__INCLUDE