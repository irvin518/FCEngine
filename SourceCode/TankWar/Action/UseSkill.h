#ifndef USERSKILL_H_INCLUDE
#define USERSKILL_H_INCLUDE
#include "ActionBase.h"

class CSkill;
class CUseSkill : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CUseSkill, 0x77329CC2, CActionBase)
public:
    CUseSkill();
    virtual ~CUseSkill();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);
private:
    std::vector<CSkill*> m_skillList;
};

#endif//!USERSKILL_H_INCLUDE
