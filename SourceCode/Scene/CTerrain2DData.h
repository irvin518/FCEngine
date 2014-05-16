#ifndef TERRAIN2DDATA_H_INCLUDE
#define TERRAIN2DDATA_H_INCLUDE

//texture Coordinate
/*
     (0,0)--------(0,width)
         |        |
         |        |
         |        |
(height,0)--------(width,height)
*/

typedef unsigned int TUInt;

enum class EGridDataType
{
    eGT_NOTPASS = 1<<24,
    eGT_PASS,
    eGT_SPEEDUP,
    eGT_SPEEDDOWN
};

typedef struct STerrainGrid
{
    unsigned int index;
    EGridDataType type;
}TTerrainGrid, *TTerrainGridPtr;

class CTerrain2DData
{
public:
    CTerrain2DData();
    ~CTerrain2DData();

    TUInt GetGridCount() const;

    void SetGrid( TUInt texWidth, TUInt texHeight, TUInt xGridCount, TUInt yGridCount );

    TTerrainGridPtr GetGrid( kmVec2& position );

    EGridDataType GetGridType( TTerrainGridPtr pGrid );
    void SetGridType( TTerrainGridPtr pGrid, EGridDataType type );

    TTerrainGridPtr GetLeftGrid( TTerrainGridPtr pGrid );
    TTerrainGridPtr GetUpGrid( TTerrainGridPtr pGrid );
    TTerrainGridPtr GetRightGrid( TTerrainGridPtr pGrid );
    TTerrainGridPtr GetDownGrid( TTerrainGridPtr pGrid );

private:
    void ResetGridData();
    void CalcXYInTable( TUInt index, TUInt& x, TUInt& y );
private:
    TTerrainGridPtr* m_pTerrainGridTable;
    TUInt m_iCount;
    TUInt m_iXGridCount;
    TUInt m_iYGridCount;
    float m_fGridWidth;
    float m_fGridHeight;
};


#endif//TERRAIN2DDATA_H_INCLUDE
