#ifndef __ControlLayer_H__
#define __ControlLayer_H__

#include "cocos2d.h"
#include "NoTouchLayer.h"
USING_NS_CC;

const int MAX_SCORE=1000000000;

class ControlLayer:public cocos2d::Layer
{
public:
	ControlLayer();
	~ControlLayer();

	virtual bool init();

	CREATE_FUNC(ControlLayer);

	void menuPauseCallback(Object* pSender);

	void updateScore(int score);

	NoTouchLayer* m_noTouchLayer;

	LabelBMFont* m_scoreItem;

	MenuItemSprite* m_pauseItem;

	Sprite* m_pauseNormal;
	Sprite* m_pausePressed;
};


#endif //__ControlLayer_H__