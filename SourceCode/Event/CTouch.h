#ifndef TOUCH_H_INCLUDE
#define TOUCH_H_INCLUDE


class CTouch
{
public:
    CTouch( int id = 0 );
    ~CTouch();
    CTouch( CTouch&& other );

    CTouch& operator=( CTouch&& other );
public:

    void UpdateTouchInfo( int id, float x, float y );

    //all point is the device touch location coordinates
    const kmVec2& GetStartPoint();

    const kmVec2& GetCurrentPoint();

    const kmVec2& GetPreviousPoint();

    float GetDelta();
private:

    int m_id;
    kmVec2 m_ve2StartPoint;
    kmVec2 m_ve2Point;
    kmVec2 m_ve2PreviousPoint;
    bool m_bStart;
};

#endif//TOUCH_H_INCLUDE