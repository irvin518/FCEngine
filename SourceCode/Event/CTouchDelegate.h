#ifndef TOUCHRECEIVERPOOH_H_INCLUDE
#define TOUCHRECEIVERPOOH_H_INCLUDE

#include "PlatformConfig.h"
#include "EventType.h"
#include "Event/EventDispatcher.h"

class CTouch;
class CTouchEvent;

class CEventDispatcher;

class CTouchDelegate : public CEventDispatcher
{
    BEATS_DECLARE_SINGLETON(CTouchDelegate);
    typedef std::map< intptr_t, int > TTouchMap;
public:

    void OnTouchBegin( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint );

    void OnTouchMove( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint );

    void OnTouchEnd( unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint );

    void OnTouchCancel(unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint);


    void SetEnable( bool bEnable );
    bool GetEnable();
private:
    int GetUnUsedIndex();

    void RemoveUsedIndex( int index );

    void OnTouchEndOrCancel( EEventType type, unsigned int num, intptr_t* pTouchIdByDevice, float* xTouchPoint, float* yTouchPoint );

private:
    TTouchMap m_currentTouchMap;
    CTouch* m_pTouches[ TOUCH_MAX_NUM ];
    unsigned int m_indexBitsUsed;
    bool m_bEnableTouch;
};


#endif//TOUCHRECEIVERPOOH_H_INCLUDE