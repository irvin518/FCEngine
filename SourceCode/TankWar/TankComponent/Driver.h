#ifndef DRIVER_H__INCLUDE
#define DRIVER_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "AdditiveAttribute.h"

class CSkill;

class CDriver : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CDriver, 0xC612B38F, CComponentBase);
public: 
	CDriver();
	virtual ~CDriver();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    CAdditiveAttribute*  m_pAttribute;
    std::vector<CSkill*> m_vSkillList;
};

class CDriverConfigs : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CDriverConfigs, 0xC612B48F, CComponentBase);
public:
    CDriverConfigs();
   virtual ~CDriverConfigs();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    std::vector<CDriver*> m_vDriverList;
};

#endif //DRIVER_H__INCLUDE