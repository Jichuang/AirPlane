#include "UFOLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

UFOLayer::UFOLayer()
{
	m_pAllMutiBullets=Array::create();
	m_pAllMutiBullets->retain();
	m_pAllBigBoom=Array::create();
	m_pAllBigBoom->retain();
}

UFOLayer::~UFOLayer()
{
	m_pAllMutiBullets->release();
	m_pAllMutiBullets=NULL;
	m_pAllBigBoom->release();
	m_pAllBigBoom=NULL;
}

bool UFOLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	//add muti
	this->schedule(schedule_selector(UFOLayer::addMutiBullets),40.0f);

	//add big boom
	this->schedule(schedule_selector(UFOLayer::addBigBoom),70.0f,kRepeatForever,5.0f);

	return true;
}

void UFOLayer::addMutiBullets(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/out_porp.wav");

	//创建多子弹道具精灵
	Sprite* mutiBullets=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("ufo1.png"));

	//获得随机位置
	Size mutiBulletsSize=mutiBullets->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=mutiBulletsSize.width/2;
	int maxX=winSize.width-mutiBulletsSize.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	//添加到UFO层并加入 Array数组
	mutiBullets->setPosition(Point(actualX,winSize.height+mutiBulletsSize.height/2));
	this->addChild(mutiBullets);
	this->m_pAllMutiBullets->addObject(mutiBullets);

	//动画
	MoveBy* move1=MoveBy::create(1.5f,Point(0,-250));
	MoveBy* move2=MoveBy::create(1.5f,Point(0,120));
	MoveBy* move3=MoveBy::create(2.5f,Point(0,-winSize.height-mutiBulletsSize.height/2));

	CallFuncN* moveDone=CallFuncN::create(this,callfuncN_selector(UFOLayer::mutiBulletsMoveFinished));
	//CallFuncN* moveDone=CallFuncN::create(CC_CALLBACK_1(UFOLayer::mutiBulletsMoveFinished,this));

	FiniteTimeAction* sequence=Sequence::create(move1,move2,move3,moveDone,NULL);

	mutiBullets->runAction(sequence);
}

//没吃到道具
void UFOLayer::mutiBulletsMoveFinished(Node* pSender)
{
	Sprite* mutiBullets=(Sprite*)pSender;
	this->removeChild(mutiBullets,true);//从屏幕中移除
	this->m_pAllMutiBullets->removeObject(mutiBullets);//从数组中移除
}

//吃到双排子道具
void UFOLayer::removeMutiBullets(Sprite* mutiBullets)
{
	this->removeChild(mutiBullets,true);
	this->m_pAllMutiBullets->removeObject(mutiBullets);
}

void UFOLayer::addBigBoom(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/out_porp.wav");

	Sprite* bigBoom=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("ufo2.png"));

	Size bigBoomSize=bigBoom->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=bigBoomSize.width/2;
	int maxX=winSize.width-bigBoomSize.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	bigBoom->setPosition(Point(actualX,winSize.height+bigBoomSize.height/2));
	this->addChild(bigBoom);
	this->m_pAllBigBoom->addObject(bigBoom);

	MoveBy* move1=MoveBy::create(1.5f,Point(0,-250));
	MoveBy* move2=MoveBy::create(1.5f,Point(0,120));
	MoveBy* move3=MoveBy::create(2.5f,Point(0,-winSize.height-bigBoomSize.height/2));

	CallFuncN* moveDone=CallFuncN::create(this,callfuncN_selector(UFOLayer::bigBoomMoveFinished));

	FiniteTimeAction* sequence=Sequence::create(move1,move2,move3,moveDone,NULL);

	bigBoom->runAction(sequence);
}

void UFOLayer::bigBoomMoveFinished(Node* pSender)
{
	Sprite* bigBoom=(Sprite*)pSender;
	this->removeChild(bigBoom,true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}

void UFOLayer::removeBigBoom(Sprite* bigBoom)
{
	this->removeChild(bigBoom,true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}


