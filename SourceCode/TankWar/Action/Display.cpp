#include "stdafx.h"
#include "Display.h"

CDisplay::CDisplay()
{

}

CDisplay::~CDisplay()
{

}

void CDisplay::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
}

void CDisplay::Execute(SAIPackage& package)
{
    package.pSourceObj->Display();
}
