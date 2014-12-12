#ifndef __NoTouchLayer_H_
#define __NoTouchLayer_H_

#include "cocos2d.h"
USING_NS_CC;
class NoTouchLayer:public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(NoTouchLayer);

	virtual bool onTouchBegan(Touch* pTouch,Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch,Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch,Event* pEvent);

};


#endif //__NoTouchLayer_H_