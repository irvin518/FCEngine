#include "stdafx.h"
#include "Die.h"

CDie::CDie()
{

}

CDie::~CDie()
{

}

void CDie::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
}

void CDie::Execute(CTank& tank)
{
    tank.Die();
}
