#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "cocos2d.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "EnemyLayer.h"
#include "UFOLAyer.h"
#include "ControlLayer.h"
#include "MutiBulletsLayer.h"

USING_NS_CC;

const int ENEMY1_SCORE=100;
const int ENEMY2_SCORE=250;
const int ENEMY3_SCORE=500;

const int MAX_BIGBOOM_COUNT=1000000; //最多存放的炸弹数（拥有:未使用）

const int TAG_BIGBOOM_MENUITEM=1;
const int TAG_BIGBOOMCOUINT_LABEL=2;

class GameLayer:public cocos2d::Layer
{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	bool onTouchBegan(Touch* pTouch,Event* pEvent);
	void onTouchMoved(Touch* pTouch,Event* pEvent);

	static GameLevel getCurLevel();

	void update(float dt);

	void updateBigBoom(int bigBoomCount);

	//使用炸弹
	void menuBigBoomCallBack(Object* pSender);

private:
	// planeLayer
	PlaneLayer* m_planeLayer;

	// bulletLayer
	BulletLayer* m_bulletLayer;

	//enemyLayer
	EnemyLayer* m_enemyLayer;

	//UFO
	UFOLayer* m_UFOLayer;

	//MutiBulletsLayer
	MutiBulletsLayer* m_mutiBuletLayer;

	//ControlLayer
	ControlLayer* m_controlLayer;

	int m_score;
	int m_bigBoomCount;


	static GameLevel level;
};


#endif  //__GameLayer_H__