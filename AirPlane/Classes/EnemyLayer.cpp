#include "Enemylayer.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

EnemyLayer::EnemyLayer():
	enemy1SpriteFrame(NULL),
	enemy2SpriteFrame(NULL),
	enemy3SpriteFrame_1(NULL),
	enemy3SpriteFrame_2(NULL)
{
	m_pAllEnemy1=Array::create();
	m_pAllEnemy1->retain();

	m_pAllEnemy2=Array::create();
	m_pAllEnemy2->retain();

	m_pAllEnemy3=Array::create();
	m_pAllEnemy3->retain();
}

EnemyLayer::~EnemyLayer()
{
	m_pAllEnemy1->release();
	m_pAllEnemy2->release();
	m_pAllEnemy3->release();

	m_pAllEnemy1=NULL;
	m_pAllEnemy2=NULL;
	m_pAllEnemy3=NULL;
}

bool EnemyLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	//加载飞机
	enemy1SpriteFrame=SpriteFrameCache::getInstance()->spriteFrameByName("enemy1.png");
	enemy2SpriteFrame=SpriteFrameCache::getInstance()->spriteFrameByName("enemy2.png");
	enemy3SpriteFrame_1=SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_n1.png");
	enemy3SpriteFrame_2=SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_n2.png");

	//飞机爆炸动画
	Animation* animation1=Animation::create();
	animation1->setDelayPerUnit(0.1f);
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down1.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down2.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down3.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down4.png"));


	Animation* animation2=Animation::create();
	animation2->setDelayPerUnit(0.1f);
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down1.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down2.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down3.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down4.png"));

	Animation* animation3=Animation::create();
	animation3->setDelayPerUnit(0.1f);
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down1.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down2.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down3.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down4.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down5.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down6.png"));

	AnimationCache::getInstance()->addAnimation(animation1,"Enemy1Blowup");
	AnimationCache::getInstance()->addAnimation(animation2,"Enemy2Blowup");
	AnimationCache::getInstance()->addAnimation(animation3,"Enemy3Blowup");

	this->schedule(schedule_selector(EnemyLayer::addEnemy1),1.5f);
	this->schedule(schedule_selector(EnemyLayer::addEnemy2),3.0f);
	this->schedule(schedule_selector(EnemyLayer::addEnemy3),6.0f);

	return true;
}

void EnemyLayer::addEnemy1(float dt)
{
	//调用绑定敌机 1
	Enemy* enemy1=Enemy::create();
	enemy1->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame),Enemy1_MaxLife);

	//随机初始位置
	Size enemy1Size=enemy1->getSprite()->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=enemy1Size.width/2;
	int maxX=winSize.width-enemy1Size.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	enemy1->setPosition(Point(actualX,winSize.height+enemy1Size.height/2));
	addChild(enemy1);
	m_pAllEnemy1->addObject(enemy1);

	//随机飞行速度
	float minDuration,maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration=3.0;
		maxDuration=6.0;
		break;
	case MIDDLE:
		minDuration=1.8;
		maxDuration=3.6;
		break;
	case HARD:
		minDuration=1.6;
		maxDuration=3.2;
		break;
	default:
		minDuration=2.0;
		maxDuration=4.0;
		break;
	}
	int rangeDuration=maxDuration-minDuration;
	int randomDuration=(rand()%rangeDuration)+minDuration;

	FiniteTimeAction* actionMove=MoveTo::create(randomDuration,Point(actualX,0-enemy1->getSprite()->getContentSize().height/2));
	//FiniteTimeAction* actionDone=CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished,this));
	//FiniteTimeAction* actionDone=CallFuncN::create(callfuncN_selector(EnemyLayer::enemy1MoveFinished));
	FiniteTimeAction* actionDone=CallFuncN::create(std::bind(&EnemyLayer::enemy1MoveFinished,this,enemy1));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	enemy1->runAction(sequence);
}

//敌机移出屏幕
void EnemyLayer::enemy1MoveFinished(Node* pSender)
{
	Enemy* enemy1=(Enemy*)pSender;
	this->removeChild(enemy1,true);
	m_pAllEnemy1->removeObject(enemy1);
}

void EnemyLayer::enemy1Blowup(Enemy* enemy1)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.wav");

	Animation* animation=AnimationCache::getInstance()->animationByName("Enemy1Blowup");
	Animate* animate=Animate::create(animation);
	CallFuncN* animationDone=CallFuncN::create(std::bind(&EnemyLayer::removeEnemy1,this,enemy1));
	Sequence* sequence=Sequence::create(animate,animationDone,NULL);
	enemy1->die();
	enemy1->getSprite()->runAction(sequence);
}

//移除一架敌机(击毁)
void EnemyLayer::removeEnemy1(Object* data)
{
	Enemy* enemy1=(Enemy*)data;
	if(enemy1!=NULL)
	{
		this->removeChild(enemy1,true);
		m_pAllEnemy1->removeObject(enemy1);
	}
}

void EnemyLayer::removeAllEnemy1()
{
	Object* obj;

	//遍历Array
	CCARRAY_FOREACH(m_pAllEnemy1,obj)
	{
		Enemy* enemy1=(Enemy*)obj;
		if(enemy1->getLife()>0)
		{
			enemy1Blowup(enemy1);
		}
	}
}

void EnemyLayer::addEnemy2(float dt)
{
	//调用绑定敌机 2
	Enemy* enemy2=Enemy::create();
	enemy2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame),Enemy2_MaxLife);

	//随机初始位置
	Size enemy2Size=enemy2->getSprite()->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=enemy2Size.width/2;
	int maxX=winSize.width-enemy2Size.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	enemy2->setPosition(Point(actualX,winSize.height+enemy2Size.height/2));
	addChild(enemy2);
	m_pAllEnemy2->addObject(enemy2);

	//随机飞行速度
	float minDuration,maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration=3.0;
		maxDuration=6.0;
		break;
	case MIDDLE:
		minDuration=2.7;
		maxDuration=5.4;
		break;
	case HARD:
		minDuration=2.5;
		maxDuration=5.0;
		break;
	default:
		minDuration=3.0;
		maxDuration=6.0;
		break;
	}
	int rangeDuration=maxDuration-minDuration;
	int randomDuration=(rand()%rangeDuration)+minDuration;

	FiniteTimeAction* actionMove=MoveTo::create(randomDuration,Point(actualX,0-enemy2->getSprite()->getContentSize().height/2));
	//FiniteTimeAction* actionDone=CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished,this));
	//FiniteTimeAction* actionDone=CallFuncN::create(callfuncN_selector(EnemyLayer::enemy1MoveFinished));
	FiniteTimeAction* actionDone=CallFuncN::create(std::bind(&EnemyLayer::enemy2MoveFinished,this,enemy2));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	enemy2->runAction(sequence);
}

//敌机移出屏幕
void EnemyLayer::enemy2MoveFinished(Node* pSender)
{
	Enemy* enemy2=(Enemy*)pSender;
	this->removeChild(enemy2,true);
	m_pAllEnemy2->removeObject(enemy2);
}

void EnemyLayer::enemy2Blowup(Enemy* enemy2)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.wav");

	Animation* animation=AnimationCache::getInstance()->animationByName("Enemy2Blowup");
	Animate* animate=Animate::create(animation);
	CallFuncN* animationDone=CallFuncN::create(std::bind(&EnemyLayer::removeEnemy2,this,enemy2));
	Sequence* sequence=Sequence::create(animate,animationDone,NULL);
	enemy2->die();
	enemy2->getSprite()->runAction(sequence);
}

//移除一架敌机(击毁)
void EnemyLayer::removeEnemy2(Node* data)
{
	Enemy* enemy2=(Enemy*)data;
	if(enemy2!=NULL)
	{
		this->removeChild(enemy2,true);
		m_pAllEnemy2->removeObject(enemy2);
	}
}

void EnemyLayer::removeAllEnemy2()
{
	Object* obj;

	//遍历Array
	CCARRAY_FOREACH(m_pAllEnemy2,obj)
	{
		Enemy* enemy2=(Enemy*)obj;
		if(enemy2->getLife()>0)
		{
			enemy2Blowup(enemy2);
		}
	}
}

void EnemyLayer::addEnemy3(float dt)
{
	//调用绑定敌机 3
	Enemy* enemy3=Enemy::create();
	enemy3->bindSprite(Sprite::createWithSpriteFrame(enemy3SpriteFrame_1),Enemy3_MaxLife);

	//随机初始位置
	Size enemy3Size=enemy3->getSprite()->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=enemy3Size.width/2;
	int maxX=winSize.width-enemy3Size.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	enemy3->setPosition(Point(actualX,winSize.height+enemy3Size.height/2));
	addChild(enemy3);
	m_pAllEnemy3->addObject(enemy3);

	//随机飞行速度
	float minDuration,maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration=4.0;
		maxDuration=8.0;
		break;
	case MIDDLE:
		minDuration=3.6;
		maxDuration=7.2;
		break;
	case HARD:
		minDuration=3.2;
		maxDuration=6.4;
		break;
	default:
		minDuration=4.0;
		maxDuration=8.0;
		break;
	}
	int rangeDuration=maxDuration-minDuration;
	int randomDuration=(rand()%rangeDuration)+minDuration;

	FiniteTimeAction* actionMove=MoveTo::create(randomDuration,Point(actualX,0-enemy3->getSprite()->getContentSize().height/2));
	//FiniteTimeAction* actionDone=CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished,this));
	//FiniteTimeAction* actionDone=CallFuncN::create(callfuncN_selector(EnemyLayer::enemy1MoveFinished));
	FiniteTimeAction* actionDone=CallFuncN::create(std::bind(&EnemyLayer::enemy3MoveFinished,this,enemy3));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	enemy3->runAction(sequence);
}

//敌机移出屏幕
void EnemyLayer::enemy3MoveFinished(Node* pSender)
{
	Enemy* enemy3=(Enemy*)pSender;
	this->removeChild(enemy3,true);
	m_pAllEnemy3->removeObject(enemy3);
}

void EnemyLayer::enemy3Blowup(Enemy* enemy3)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");

	Animation* animation=AnimationCache::getInstance()->animationByName("Enemy3Blowup");
	Animate* animate=Animate::create(animation);
	CallFuncN* animationDone=CallFuncN::create(std::bind(&EnemyLayer::removeEnemy3,this,enemy3));
	Sequence* sequence=Sequence::create(animate,animationDone,NULL);
	enemy3->die();
	enemy3->getSprite()->runAction(sequence);
}

//移除一架敌机(击毁)
void EnemyLayer::removeEnemy3(Node* data)
{
	Enemy* enemy3=(Enemy*)data;
	if(enemy3!=NULL)
	{
		this->removeChild(enemy3,true);
		m_pAllEnemy3->removeObject(enemy3);
	}
}

void EnemyLayer::removeAllEnemy3()
{
	Object* obj;

	//遍历Array
	CCARRAY_FOREACH(m_pAllEnemy3,obj)
	{
		Enemy* enemy3=(Enemy*)obj;
		if(enemy3->getLife()>0)
		{
			enemy3Blowup(enemy3);
		}
	}
}

void EnemyLayer::removeAllEnemy()
{
	removeAllEnemy1();
	removeAllEnemy2();
	removeAllEnemy3();
}