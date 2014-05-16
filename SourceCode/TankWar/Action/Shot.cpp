#include "stdafx.h"
#include "Shot.h"
#include "../TankComponent/Turret.h"

CShot::CShot()
{

}


CShot::~CShot()
{

}

void CShot::ReflectData( CSerializer& serializer )
{

}

void CShot::Execute(SAIPackage& package)
{
    package.pSourceObj->GetTurret()->Shot();
}
