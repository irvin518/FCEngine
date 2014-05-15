#ifndef TOUCHEVENT_H_INCLUDE
#define TOUCHEVENT_H_INCLUDE

#include "Event/BaseEvent.h"

class CTouch;

class CTouchEvent : public CBaseEvent
{
    typedef std::vector< CTouch* > TTouchVector;
public:
    CTouchEvent( int type );
    ~CTouchEvent();

    CTouchEvent( CTouchEvent&& other );
    CTouchEvent& operator=( CTouchEvent&& other );

    int GetTouchNum();
    CTouch** GetTouchQueue();

    void AddTouch( CTouch* pTouch );

private:
    int m_num;
    TTouchVector m_touchVector;
};

#endif//TOUCHEVENT_H_INCLUDE