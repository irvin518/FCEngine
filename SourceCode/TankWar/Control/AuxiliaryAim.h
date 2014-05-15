#ifndef AUXILIARYAIM_H__INCLUDE
#define AUXILIARYAIM_H__INCLUDE


#include "Box2D/Box2D.h"
//#include "TankWar/Physics/physicsBody.h"

class CAuxiliaryAim : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CAuxiliaryAim, 0xABCDB41, CComponentBase)
public:
	CAuxiliaryAim();
	~CAuxiliaryAim();
    bool Aim( float x, float y );

#ifndef USE_BOX2D_PHYSICE
    bool Scan( float x, float y );
#endif // USE_BOX2D_PHYSICE

};

#endif //AUXILIARYAIM_H__INCLUDE