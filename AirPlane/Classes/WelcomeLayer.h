#ifndef __WelcomeLayer_H__
#define __WelcomeLayer_H__

#include "cocos2d.h"
USING_NS_CC;

class WelcomeLayer:public cocos2d::Layer
{
public:

	CREATE_FUNC(WelcomeLayer);
	virtual bool init();

	void menuCloseCallback(Object *pSender);

	bool isHaveSaveFile();
	void getHighesHistoryScore();
};

#endif //__WelcomeLayer_H__