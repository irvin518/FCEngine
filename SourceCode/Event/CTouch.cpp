
#include "stdafx.h"
#include "CTouch.h"


CTouch::CTouch( int id )
{
    m_id = id;
    m_bStart = false;
    kmVec2Fill( &m_ve2Point, 0.0, 0.0 );
    kmVec2Fill( &m_ve2PreviousPoint, 0.0, 0.0 );
    kmVec2Fill( &m_ve2StartPoint, 0.0, 0.0 );
}

CTouch::CTouch( CTouch&& other )
{
    *this = std::move( other );
}

CTouch::~CTouch()
{

}

void CTouch::UpdateTouchInfo( int id, float x, float y )
{
    BEATS_ASSERT( m_id == id, _T("Is touch id right?"));
    kmVec2Fill( &m_ve2PreviousPoint, m_ve2Point.x, m_ve2Point.y );
    kmVec2Fill( &m_ve2Point, x, y );
    if ( !m_bStart )
    {
        kmVec2Fill( &m_ve2StartPoint, x, y );
        kmVec2Fill( &m_ve2PreviousPoint, m_ve2Point.x, m_ve2Point.y );
        m_bStart = true;
    }
}

const kmVec2& CTouch::GetStartPoint()
{
    return m_ve2StartPoint;
}

const kmVec2& CTouch::GetCurrentPoint()
{
    return m_ve2Point;
}

const kmVec2& CTouch::GetPreviousPoint()
{
    return m_ve2PreviousPoint;
}

float CTouch::GetDelta()
{
    kmVec2 subV;
    kmVec2Subtract( &subV, &m_ve2Point, &m_ve2PreviousPoint );
    return kmVec2Length( &subV );
}

CTouch& CTouch::operator=( CTouch&& other )
{
    m_id = other.m_id;
    m_bStart = other.m_bStart;
    kmVec2Fill( &m_ve2Point, other.m_ve2Point.x, other.m_ve2Point.y );
    kmVec2Fill( &m_ve2PreviousPoint, other.m_ve2PreviousPoint.x, other.m_ve2PreviousPoint.y );
    kmVec2Fill( &m_ve2StartPoint, other.m_ve2StartPoint.x, other.m_ve2StartPoint.y );
    return *this;
}
