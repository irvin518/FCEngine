
#include "stdafx.h"
#include "CTouchDelegate.h"
#include "CTouchEvent.h"
#include "CTouch.h"

CTouchDelegate* CTouchDelegate::m_pInstance = nullptr;

CTouchDelegate::CTouchDelegate()
{
    m_indexBitsUsed = 0;
    m_bEnableTouch = true;
}
CTouchDelegate::~CTouchDelegate()
{
}

void CTouchDelegate::OnTouchBegin( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint )
{
    if ( m_bEnableTouch )
    {
        intptr_t id = 0;
        CTouchEvent touchEvent(EEventType::eEVT_EVENT_TOUCH_BEGIN);
        for ( unsigned int index = 0; index < num ; ++index )
        {
            id = pTouchIdByDevice[ index ];
            auto iter = m_currentTouchMap.find( id );
            if ( iter == m_currentTouchMap.end() )
            {
                int useIndex = 0;
                useIndex = GetUnUsedIndex();
                if ( -1 == useIndex )
                {
                    continue;
                }
                CTouch* pTouch = new CTouch( id );
                m_pTouches[ useIndex ] = pTouch;
                pTouch->UpdateTouchInfo( id, xTouchPoint[ index ], yTouchPoint[ index ] );
                touchEvent.AddTouch( pTouch );
                m_currentTouchMap.insert( std::make_pair( id, iter->second ));
            }
        }

        DispatchEvent( &touchEvent );
    }
}

void CTouchDelegate::OnTouchMove( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint )
{
    if ( m_bEnableTouch )
    {
        intptr_t id = 0;
        CTouchEvent touchEvent(EEventType::eEVT_EVENT_TOUCH_MOVE);
        for ( unsigned int index = 0; index < num ; ++index )
        {
            id = pTouchIdByDevice[ index ];
            auto iter = m_currentTouchMap.find( id );
            if ( iter != m_currentTouchMap.end() )
            {
                CTouch* pTouch = m_pTouches[ iter->second ];
                pTouch->UpdateTouchInfo( id, xTouchPoint[ index ], yTouchPoint[ index ] );
                touchEvent.AddTouch( pTouch );
            }
        }

        DispatchEvent( &touchEvent );
    }
}

void CTouchDelegate::OnTouchEnd( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint )
{
    OnTouchEndOrCancel( EEventType::eEVT_EVENT_TOUCH_END, num, pTouchIdByDevice, xTouchPoint, yTouchPoint );
}

void CTouchDelegate::OnTouchCancel( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint )
{
    OnTouchEndOrCancel( EEventType::eEVT_EVENT_TOUCH_CANCEL, num, pTouchIdByDevice, xTouchPoint, yTouchPoint );
}

int CTouchDelegate::GetUnUsedIndex()
{
    int i = -1;
    int temp = m_indexBitsUsed;

    for (i = 0; i < TOUCH_MAX_NUM; i++)
    {
        if (! (temp & 0x00000001))
        {
            m_indexBitsUsed |= (1 <<  i);
            break;
        }
        temp >>= 1;
    }

    // all bits are used
    return i;
}

void CTouchDelegate::RemoveUsedIndex( int index )
{
    if (index < 0 || index >= TOUCH_MAX_NUM )
    {
        return;
    }

    unsigned int temp = 1 << index;
    temp = ~temp;
    m_indexBitsUsed &= temp;
}

void CTouchDelegate::OnTouchEndOrCancel( EEventType type, unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint )
{
    if ( m_bEnableTouch )
    {
        intptr_t id = 0;
        CTouchEvent touchEvent( type );
        for ( unsigned int index = 0; index < num ; ++index )
        {
            id = pTouchIdByDevice[ index ];
            auto iter = m_currentTouchMap.find( id );
            if ( iter != m_currentTouchMap.end() )
            {
                CTouch* pTouch = m_pTouches[ iter->second ];
                pTouch->UpdateTouchInfo( id, xTouchPoint[ index ], yTouchPoint[ index ] );
                touchEvent.AddTouch( pTouch );

                m_currentTouchMap.erase( iter );
                RemoveUsedIndex( iter->second );
                BEATS_SAFE_DELETE( pTouch );
            }
        }

        DispatchEvent( &touchEvent );
    }
}

void CTouchDelegate::SetEnable( bool bEnable )
{
    m_bEnableTouch = bEnable;
}

bool CTouchDelegate::GetEnable()
{
    return m_bEnableTouch;
}

