#ifndef __GameOverLayer_H__
#define __GameOverLayer_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer:public cocos2d::Layer
{
public:
	GameOverLayer();
	~GameOverLayer();

	virtual bool init();

	static GameOverLayer* create(int passScore);

	void menuBackCallback(Object* pSender);

	void beginChangeHighestScore(Node* pNode);

	void showAD();

	void menuExit(Object* obj);

public:
	int m_score;

	static int m_highestHistoryScore;

	LabelBMFont* m_highestScore;
};


#endif //__GameOverLayer_H__