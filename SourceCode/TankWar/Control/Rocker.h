#ifndef ROCKER_H__INCLUDE
#define ROCKER_H__INCLUDE


class CRocker //: public Layer 
{
public :    
	CRocker();
	~CRocker();
	//The initialization of tPoint is rocker center fRadius is rocker radius pJsSprite is rocker control point pJsBg is rocker background
// 	static CRocker*  CreateCRockerWithCenter(Point tPoint ,float fRadius ,Sprite* pJsSprite,Sprite* pJsBg,bool bIsFollowRole);
// 	void Active(); //Start the rocker
// 	void Inactive(); //End the rocker 
// private:
// 	CRocker * initWithCenter(Point tPoint ,float fRadius ,Sprite* pJsSprite,Sprite* pJsBg,bool bIsFollowRole);
// 	Point getDirection();
// 	float getVelocity();
// 	void  updatePos(float dt);
// 
// 	Point m_tCenterPoint;
// 	Point m_tCurrentPoint;
// 	bool m_bActive;
// 	float m_fRadius;
// 	Sprite *m_pJsSprite;
// 	Sprite *m_pJsBg;
// 	bool m_bIsFollowRole;
// 
// 	virtual bool ccToucheBegan(Touch *pTouch, Event *pEvent);
// 	virtual void ccToucheMoved(Touch *pTouch, Event *pEvent);
// 	virtual void ccToucheEnded(Touch *pTouch, Event *pEvent);

};

#endif //ROCKER_H__INCLUDE
