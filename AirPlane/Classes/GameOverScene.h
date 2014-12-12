#ifndef __GameOverScene_H__
#define __GameOverScene_H__

#include "cocos2d.h"
#include "GameOverLayer.h"

USING_NS_CC;

class GameOverScene:public cocos2d::Scene
{
public:
	GameOverScene();
	~GameOverScene();

	virtual bool init();

	static GameOverScene* create(int passScore);

private:
	GameOverLayer* m_gameOverLayer;

	int m_score;
};


#endif //__GameOverScene_H__