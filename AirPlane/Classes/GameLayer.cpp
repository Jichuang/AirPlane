#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

GameLevel GameLayer::level=EASY;

GameLayer::GameLayer():
	m_planeLayer(NULL),
	m_bulletLayer(NULL),
	m_enemyLayer(NULL),
	m_mutiBuletLayer(NULL),
	m_UFOLayer(NULL),
	m_score(0),
	m_bigBoomCount(0)
{

}
GameLayer::~GameLayer()
{

}

bool GameLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	level=EASY;

	//background mp3 (游戏结束后重新开始)
	if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav",true);
	}

	//add planeLayer
	this->m_planeLayer=PlaneLayer::create();
	this->addChild(m_planeLayer);

	//add bulletLayer
	this->m_bulletLayer=BulletLayer::create();
	this->addChild(m_bulletLayer);
	this->m_bulletLayer->startShoot(1.0f);

	//enemyLayer
	this->m_enemyLayer=EnemyLayer::create();
	this->addChild(m_enemyLayer);

	//UFO
	this->m_UFOLayer=UFOLayer::create();
	this->addChild(m_UFOLayer);

	//MutiBulletsLayer
	this->m_mutiBuletLayer=MutiBulletsLayer::create();
	this->addChild(m_mutiBuletLayer);

	//controlLayer
	this->m_controlLayer=ControlLayer::create();
	this->addChild(m_controlLayer);

	this->scheduleUpdate();

	//touch event
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(GameLayer::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(GameLayer::onTouchMoved,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

bool GameLayer::onTouchBegan(Touch* pTouch,Event* pEvent)
{
	return true;
}

//移动飞机精灵
void GameLayer::onTouchMoved(Touch* pTouch,Event* pEvent)
{
	if(this->m_planeLayer->m_isAlive)
	{
		//屏幕坐标 左上角(0,0) 
		Point beginPoint=pTouch->getLocationInView();
		beginPoint=Director::getInstance()->convertToGL(beginPoint);//获取触摸点坐标

		//juggle the area of drag
		//获取飞机当前位置 形状位置
		Rect planeRect=m_planeLayer->getChildByTag(Tag_Air_Plane)->boundingBox();
		planeRect.origin.x-=15;
		planeRect.origin.y-=15;
		planeRect.size.width+=30;
		planeRect.size.height+=30;//允许稍微加大一点触摸位置，游戏实际需要

		//判断是否在飞机触摸范围内
		if(planeRect.containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)))
		{
			Point endPoint=pTouch->getPreviousLocationInView();//获取触摸的前一个位置
			endPoint=Director::getInstance()->convertToGL(endPoint);

			Point offSet=ccpSub(beginPoint,endPoint);//获取 offSet :偏移量
			//获取真正移动的位置
			Point toPoint=ccpAdd(this->m_planeLayer->getChildByTag(Tag_Air_Plane)->getPosition(),offSet);
			this->m_planeLayer->moveTo(toPoint); //移动飞机
		}
	}
}

//返回游戏级别(敌机移动的速度)
GameLevel GameLayer::getCurLevel()
{
	return level;
}

//碰撞检测
void GameLayer::update(float dt)
{
	if(level==EASY && m_score>=100000)
	{
		level=MIDDLE;
	}
	else if(level==MIDDLE && m_score>=500000)
	{
		level=HARD;
	}

	//创建一个Array数组，用于存放代删除的子弹，也就是此帧中被检测到碰撞的子弹
	Array* bulletsToDelete=Array::create();
	bulletsToDelete->retain();
	Object* bt,*ut,*et;

	//enemy1 && bullet CheckCollosion
	CCARRAY_FOREACH(this->m_bulletLayer->m_pAllBullet,bt)//遍历所有子弹
	{
		Sprite* bullet=(Sprite*)bt;

		Array* enemy1ToDelete=Array::create();//存放代删除的敌机
		enemy1ToDelete->retain();
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if(bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))// 碰撞检测，即矩形区域是否有相交
			{
				if(enemy1->getLife()==1)//enemy1 设置了一滴血(碰到就挂了)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1ToDelete->addObject(enemy1);
				}
			}
		}
		CCARRAY_FOREACH(enemy1ToDelete,et) //遍历次帧中所有碰撞死亡的敌机
		{
			Enemy* enemy1=(Enemy*)et;
			this->m_enemyLayer->enemy1Blowup(enemy1);//执行爆炸
			m_score+=ENEMY1_SCORE;
			this->m_controlLayer->updateScore(m_score);
		}
		enemy1ToDelete->release();
	}

	CCARRAY_FOREACH(bulletsToDelete,bt)//遍历次帧中遍历的子弹
	{
		Sprite* bullet=(Sprite*)bt;
		this->m_bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	
	//enemy2 && bullet
	CCARRAY_FOREACH(this->m_bulletLayer->m_pAllBullet,bt)
	{
		Sprite* bullet=(Sprite*)bt;

		Array* enemy2ToDelete=Array::create();
		enemy2ToDelete->retain();
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if(bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if(enemy2->getLife()>1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					enemy2ToDelete->addObject(enemy2);
					bulletsToDelete->addObject(bullet);
				}
				else ; //爆炸，敌机还未消失，子弹还会打到敌机，所有不进行处理
			}
		}
		CCARRAY_FOREACH(enemy2ToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->m_enemyLayer->enemy2Blowup(enemy2);//执行爆炸效果
			m_score+=ENEMY2_SCORE;
			this->m_controlLayer->updateScore(m_score);
		}
		enemy2ToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		Sprite* bullet=(Sprite*)bt;
		this->m_bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy3 && bullet
	CCARRAY_FOREACH(this->m_bulletLayer->m_pAllBullet,bt)
	{
		Sprite* bullet=(Sprite*)bt;

		Array* enemy3ToDelete=Array::create();
		enemy3ToDelete->retain();
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if(bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if(enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					enemy3ToDelete->addObject(enemy3);
					bulletsToDelete->addObject(bullet);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3ToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->m_enemyLayer->enemy3Blowup(enemy3);
			m_score+=ENEMY3_SCORE;
			this->m_controlLayer->updateScore(m_score);
		}
		enemy3ToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		Sprite* bullet=(Sprite*)bt;
		this->m_bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	bulletsToDelete->release();

	//mutiBullet and enemy (双排子弹)
	Array* mutiBulletToDelete=Array::create();
	mutiBulletToDelete->retain();
	Object* mbt;
	//enemy1 and muti_bullet
	CCARRAY_FOREACH(this->m_mutiBuletLayer->m_pAllMutiBulletArr,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		Array* enemy1ToDelete=Array::create();
		enemy1ToDelete->retain();
		//碰撞
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if(mutiBullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if(enemy1->getLife()==1)
				{
					enemy1->loseLife();
					mutiBulletToDelete->addObject(mutiBullet);
					enemy1ToDelete->addObject(enemy1);
					m_score+=ENEMY1_SCORE;
					this->m_controlLayer->updateScore(m_score);
				}
			}
		}
		//blowup
		CCARRAY_FOREACH(enemy1ToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->m_enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1ToDelete->release();
	}
	//移除muti子弹
	CCARRAY_FOREACH(mutiBulletToDelete,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		this->m_mutiBuletLayer->mutiBulletRemove(mutiBullet);
	}
	mutiBulletToDelete->removeAllObjects();

	//enemy2 and muti_bullet
	CCARRAY_FOREACH(this->m_mutiBuletLayer->m_pAllMutiBulletArr,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		Array* enemy2ToDelete=Array::create();
		enemy2ToDelete->retain();
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if(mutiBullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if(enemy2->getLife()>1)
				{
					enemy2->loseLife();
					mutiBulletToDelete->addObject(mutiBullet);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					enemy2ToDelete->addObject(enemy2);
					mutiBulletToDelete->addObject(mutiBullet);
					m_score+=ENEMY2_SCORE;
					this->m_controlLayer->updateScore(m_score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2ToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->m_enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2ToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletToDelete,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		this->m_mutiBuletLayer->mutiBulletRemove(mutiBullet);
	}
	mutiBulletToDelete->removeAllObjects();

	//enemy3 and muti_bullet
	CCARRAY_FOREACH(this->m_mutiBuletLayer->m_pAllMutiBulletArr,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		Array* enemy3ToDelete=Array::create();
		enemy3ToDelete->retain();
		CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if(mutiBullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if(enemy3->getLife()>1)
				{
					enemy3->loseLife();
					mutiBulletToDelete->addObject(mutiBullet);
				}
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					enemy3ToDelete->addObject(enemy3);
					mutiBulletToDelete->addObject(mutiBullet);
					m_score+=ENEMY3_SCORE;
					this->m_controlLayer->updateScore(m_score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3ToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->m_enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3ToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletToDelete,mbt)
	{
		Sprite* mutiBullet=(Sprite*)mbt;
		this->m_mutiBuletLayer->mutiBulletRemove(mutiBullet);
	}
	mutiBulletToDelete->removeAllObjects();
	mutiBulletToDelete->release();

	//碰撞检测 （飞机与敌机）
	Rect airPlaneRect=this->m_planeLayer->getChildByTag(Tag_Air_Plane)->boundingBox();
	airPlaneRect.origin.x+=30;
	airPlaneRect.size.width-=60;

	//enemy1
	CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy1,et)
	{
		Enemy* enemy1=(Enemy*)et;
		if(enemy1->getLife()>0)
		{
			if(airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->m_bulletLayer->stopShoot();
				this->m_mutiBuletLayer->stopShoot();
				this->m_planeLayer->blowup(m_score);
				return ;
			}
		}
	}

	//enemy2
	CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy2,et)
	{
		Enemy* enemy2=(Enemy*)et;
		if(enemy2->getLife()>0)
		{
			if(airPlaneRect.intersectsRect(enemy2->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->m_bulletLayer->stopShoot();
				this->m_mutiBuletLayer->stopShoot();
				this->m_planeLayer->blowup(m_score);
				return ;
			}
		}
	}

	//enemy3
	CCARRAY_FOREACH(this->m_enemyLayer->m_pAllEnemy3,et)
	{
		Enemy* enemy3=(Enemy*)et;
		if(enemy3->getLife()>0)
		{
			if(airPlaneRect.intersectsRect(enemy3->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->m_bulletLayer->stopShoot();
				this->m_mutiBuletLayer->stopShoot();
				this->m_planeLayer->blowup(m_score);
				return ;
			}
		}
	}

	//ufo mutiBullet
	CCARRAY_FOREACH(this->m_UFOLayer->m_pAllMutiBullets,ut)
	{
		Sprite* mutiBulletUfo=(Sprite*)ut;
		if(this->m_planeLayer->getChildByTag(Tag_Air_Plane)->boundingBox().intersectsRect(mutiBulletUfo->getBoundingBox()))
		{
			this->m_UFOLayer->removeMutiBullets(mutiBulletUfo);
			this->m_bulletLayer->stopShoot();
			this->m_mutiBuletLayer->startShoot();
			this->m_bulletLayer->startShoot(6.2f);
		}
	}

	//bigboom
	CCARRAY_FOREACH(this->m_UFOLayer->m_pAllBigBoom,bt)
	{
		Sprite* bigboom=(Sprite*)bt;
		if(this->m_planeLayer->getChildByTag(Tag_Air_Plane)->boundingBox().intersectsRect(bigboom->getBoundingBox()))
		{
			this->m_UFOLayer->removeBigBoom(bigboom);
			m_bigBoomCount++;

			updateBigBoom(m_bigBoomCount);
		}
	}
}

//更新炸弹的数目
void GameLayer::updateBigBoom(int bigBoomCount)
{
	auto normalBomb=Sprite::createWithSpriteFrameName("bomb.png");
	auto pressBomb=Sprite::createWithSpriteFrameName("bomb.png");

	if(m_bigBoomCount < 0)
	{
		return ;
	}
	else if(m_bigBoomCount==0) //
	{
		if(this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM,true);
		}
		if(this->getChildByTag(TAG_BIGBOOMCOUINT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUINT_LABEL,true);
		}
	}
	else if(m_bigBoomCount==1)
	{
		if(!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			MenuItemSprite* menuitem=MenuItemSprite::create(normalBomb,pressBomb,NULL,CC_CALLBACK_1(GameLayer::menuBigBoomCallBack,this));
			menuitem->setPosition(Point(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			Menu* menu=Menu::create(menuitem,NULL);
			menu->setPosition(Point::ZERO);
			this->addChild(menu,0,TAG_BIGBOOM_MENUITEM);
		}
		if(this->getChildByTag(TAG_BIGBOOMCOUINT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUINT_LABEL,true);
		}
	}
	else
	{
		if(!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			MenuItemSprite* menuitem=MenuItemSprite::create(normalBomb,pressBomb,NULL,CC_CALLBACK_1(GameLayer::menuBigBoomCallBack,this));
			menuitem->setPosition(Point(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			Menu* menu=Menu::create(menuitem,NULL);
			this->addChild(menu,0,TAG_BIGBOOM_MENUITEM);
		}
		if(this->getChildByTag(TAG_BIGBOOMCOUINT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUINT_LABEL,true);
		}
		if(m_bigBoomCount>=0 && m_bigBoomCount<MAX_BIGBOOM_COUNT)
		{
			String* strScore=String::createWithFormat("X%d",m_bigBoomCount);
			LabelBMFont* label=LabelBMFont::create(strScore->getCString(),"font/font.fnt");
			label->setColor(Color3B(143,146,147));
			label->setAnchorPoint(Point(0,0.5));
			label->setPosition(Point(normalBomb->getContentSize().width+15,normalBomb->getContentSize().height/2+5));
			this->addChild(label,0,TAG_BIGBOOMCOUINT_LABEL);
		}
	}
}

//使用炸弹
void GameLayer::menuBigBoomCallBack(Object* pSender)
{
	if(m_bigBoomCount>0 && !Director::getInstance()->isPaused())
	{
		m_bigBoomCount--;
		m_score+=this->m_enemyLayer->m_pAllEnemy1->count()*ENEMY1_SCORE;
		m_score+=this->m_enemyLayer->m_pAllEnemy2->count()*ENEMY2_SCORE;
		m_score+=this->m_enemyLayer->m_pAllEnemy3->count()*ENEMY3_SCORE;

		this->m_enemyLayer->removeAllEnemy();
		updateBigBoom(m_bigBoomCount);
		this->m_controlLayer->updateScore(m_score);
	}
}