#ifndef TERRAIN_H_INCLUDE
#define TERRAIN_H_INCLUDE

#include "CNode2D.h"
#include "CTerrain2DData.h"

class CTerrain2D : public CNode2D
{
public:
    CTerrain2D( const TString& name );
    virtual ~CTerrain2D();

    void LoadTerrain( const TString& textureAtlasName, const TString& textureFragName );

};

#endif//TERRAIN_H_INCLUDE


