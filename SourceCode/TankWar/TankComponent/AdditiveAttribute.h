#ifndef TANKWAR_ADDITIVEATTRIBUTE_H__INCLUDE
#define TANKWAR_ADDITIVEATTRIBUTE_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class  CAdditiveAttribute : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CAdditiveAttribute, 0xC123B88F, CComponentBase);
public:
    CAdditiveAttribute();
    virtual ~CAdditiveAttribute();
    virtual void ReflectData(CSerializer& serializer) override;
private:
    float m_fAttack;
    float m_fDefense;
    float m_fHP;
    float m_fRecSpeed;
};

#endif //TANKWAR_ADDITIVEATTRIBUTE_H__INCLUDE