
#include "stdafx.h"
#include "CTerrain2DData.h"

CTerrain2DData::CTerrain2DData()
{
    m_pTerrainGridTable = nullptr;
    m_iCount = 0;
    m_iXGridCount = 0;
    m_iYGridCount = 0;
    m_fGridWidth = 0.0f;
    m_fGridHeight = 0.0f;

}

CTerrain2DData::~CTerrain2DData()
{

}

TUInt CTerrain2DData::GetGridCount() const
{
    return m_iCount;
}

void CTerrain2DData::SetGrid( TUInt texWidth, TUInt texHeight, TUInt xGridCount, TUInt yGridCount )
{
    ResetGridData();
    m_iCount = xGridCount * yGridCount;
    m_pTerrainGridTable = new TTerrainGridPtr[ m_iCount ];
    TUInt index = 0;
    m_fGridWidth = (float)texWidth / xGridCount;
    m_fGridHeight = (float)texHeight / yGridCount;
    for ( TUInt y = 0; y < yGridCount; y++ )
    {
        for ( TUInt x = 0; x < xGridCount; ++x )
        {
            index = y * xGridCount + x;
            TTerrainGridPtr pNewGrid = new TTerrainGrid();
            pNewGrid->index = index;
            pNewGrid->type = EGridDataType::eGT_PASS;
            m_pTerrainGridTable[ index ] = pNewGrid;
        }
    }
}

TTerrainGridPtr CTerrain2DData::GetGrid( kmVec2& position )
{
    TTerrainGridPtr pGrid = nullptr;
    TUInt x = (TUInt)position.x / m_fGridWidth;
    TUInt y = (TUInt)position.y / m_fGridHeight;
    TUInt index = y * m_iXGridCount + x;
    if ( index < m_iCount )
    {
        pGrid = m_pTerrainGridTable[ index ];
    }
    return pGrid;
}

EGridDataType CTerrain2DData::GetGridType( TTerrainGridPtr pGrid )
{
    BEATS_ASSERT( pGrid );
    return pGrid->type;
}

void CTerrain2DData::SetGridType( TTerrainGridPtr pGrid, EGridDataType type )
{
    BEATS_ASSERT( pGrid );
    pGrid->type = type;
}

TTerrainGridPtr CTerrain2DData::GetLeftGrid( TTerrainGridPtr pGrid )
{
    TTerrainGridPtr pRetGrid = nullptr;
    BEATS_ASSERT( pGrid );
    TUInt x = 0, y = 0;
    CalcXYInTable( pGrid->index, x, y );
    x = x - 1;
    if ( x >= 0 )
    {
        pRetGrid = m_pTerrainGridTable[ y * m_iXGridCount + x ];
    }
    return pRetGrid;
}

TTerrainGridPtr CTerrain2DData::GetUpGrid( TTerrainGridPtr pGrid )
{
    TTerrainGridPtr pRetGrid = nullptr;
    BEATS_ASSERT( pGrid );
    TUInt x = 0, y = 0;
    CalcXYInTable( pGrid->index, x, y );
    y = y - 1;
    if ( y >= 0 )
    {
        pRetGrid = m_pTerrainGridTable[ y * m_iXGridCount + x ];
    }
    return pRetGrid;
}

TTerrainGridPtr CTerrain2DData::GetRightGrid( TTerrainGridPtr pGrid )
{
    TTerrainGridPtr pRetGrid = nullptr;
    BEATS_ASSERT( pGrid );
    TUInt x = 0, y = 0;
    CalcXYInTable( pGrid->index, x, y );
    x = x + 1;
    if ( x < m_iXGridCount )
    {
        pRetGrid = m_pTerrainGridTable[ y * m_iXGridCount + x ];
    }
    return pRetGrid;
}

TTerrainGridPtr CTerrain2DData::GetDownGrid( TTerrainGridPtr pGrid )
{
    TTerrainGridPtr pRetGrid = nullptr;
    BEATS_ASSERT( pGrid );
    TUInt x = 0, y = 0;
    CalcXYInTable( pGrid->index, x, y );
    y = y + 1;
    if ( y < m_iYGridCount )
    {
        pRetGrid = m_pTerrainGridTable[ y * m_iXGridCount + x ];
    }
    return pRetGrid;
}

void CTerrain2DData::ResetGridData()
{
    if ( m_iCount > 0 )
    {
        for ( TUInt i = 0; i < m_iCount; ++i )
        {
            BEATS_SAFE_DELETE( m_pTerrainGridTable[i] );
        }
        BEATS_SAFE_DELETE_ARRAY( m_pTerrainGridTable );
    }
}

void CTerrain2DData::CalcXYInTable( TUInt index, TUInt& x, TUInt& y )
{
    x = index % m_iXGridCount;
    y = index / m_iXGridCount;
}
