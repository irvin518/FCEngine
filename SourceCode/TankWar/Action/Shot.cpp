#include "stdafx.h"
#include "Shot.h"
#include "../TankComponent/Turret.h"

CShot::CShot()
{

}


CShot::~CShot()
{

}

void CShot::Execute(CTank& tank)
{
    tank.GetTurret()->Shot();
}

void CShot::ReflectData( CSerializer& serializer )
{

}
