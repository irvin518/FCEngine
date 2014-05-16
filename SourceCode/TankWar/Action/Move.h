#ifndef MOVE_H_INCLUDE
#define MOVE_HINCLUDE
#include "ActionBase.h"

class CMove : public CActionBase
{
    typedef CActionBase super;
    DECLARE_REFLECT_GUID(CMove, 0xFECCCB90, CActionBase)
public:
    CMove();
    virtual ~CMove();
    virtual void ReflectData(CSerializer& serializer) override;
public:
    virtual void Execute(SAIPackage& package);
private:
    kmVec2 m_direction;
};

#endif//!MOVE_HINCLUDE