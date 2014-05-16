#ifndef ACTIONBASE_H__INCLUDE
#define ACTIONBASE_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "../TankComponent/Tank.h"

struct SAIPackage
{
    CTank* pSourceObj;
    CTank* pTargetObj;
};

class CActionBase : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID_ABSTRACT(CActionBase, 0xD1AC76C0, CComponentBase)
public:
    CActionBase();
    virtual ~CActionBase();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    CActionBase* GetNextAction();
    virtual void Execute(SAIPackage& package) = 0;

protected:
    CActionBase* m_pNextAction;

};

#endif//!ACTIONBASE_H__INCLUDE
