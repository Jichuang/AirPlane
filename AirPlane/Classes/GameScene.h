#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class GameScene:public Scene
{
public:
	GameScene();
	~GameScene();

	CREATE_FUNC(GameScene);

	virtual bool init();

	GameLayer* m_gameLayer;

	void backgroundMove(float dt);

private:
	Sprite* background1;
	Sprite* background2;
};


#endif //__GameScene_H__