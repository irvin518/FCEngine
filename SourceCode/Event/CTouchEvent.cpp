
#include "stdafx.h"
#include "CTouchEvent.h"
#include "EventType.h"


CTouchEvent::CTouchEvent( int type ) :CBaseEvent( type )
{
    m_touchVector.reserve( TOUCH_MAX_NUM );
}

CTouchEvent::~CTouchEvent()
{

}

int CTouchEvent::GetTouchNum()
{
    return m_touchVector.size();
}

CTouch** CTouchEvent::GetTouchQueue()
{
    return &m_touchVector[0];
}

void CTouchEvent::AddTouch( CTouch* pTouch )
{
    m_touchVector.push_back( pTouch );
}
