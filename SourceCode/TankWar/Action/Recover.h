#ifndef RECOVER_H_INCLUDE
#define RECOVER_H_INCLUDE
#include "ActionBase.h"

class CRecover : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CRecover, 0x84487695, CActionBase)
public:
    CRecover();
    virtual ~CRecover();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);
private:
    kmScalar m_fRecoverHP;
};

#endif//!RECOVER_H_INCLUDE