#ifndef __WelcomeScene_H__
#define __WelcomeScene_H__

#include "cocos2d.h"
#include "WelcomeLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class WelcomeScene:public cocos2d::Scene
{
public:
	WelcomeScene();
	~WelcomeScene();

	virtual bool init();

	CREATE_FUNC(WelcomeScene);

	void preloadMusic();

	WelcomeLayer* m_welcomeLayer;

};


#endif //__WelcomeScene_H__