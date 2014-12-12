#include "MutiBulletsLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

MutiBulletsLayer::MutiBulletsLayer():
	mutiBulletBatchNode(NULL)
{
	m_pAllMutiBulletArr=Array::create();
	m_pAllMutiBulletArr->retain();
}

MutiBulletsLayer::~MutiBulletsLayer()
{
	m_pAllMutiBulletArr->release();
	m_pAllMutiBulletArr=NULL;
}

bool MutiBulletsLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	//加载 渲染的 精灵(双排子弹)
	Texture2D* texture=TextureCache::getInstance()->textureForKey("ui/shoot.png");
	mutiBulletBatchNode=SpriteBatchNode::createWithTexture(texture);
	this->addChild(mutiBulletBatchNode);

	return true;
}

void MutiBulletsLayer::addMutiBullet(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");

	//mutiBullet and it's action
	Sprite* bulletLeft=Sprite::createWithSpriteFrameName("bullet2.png");
	Sprite* bulletRight=Sprite::createWithSpriteFrameName("bullet2.png");

	mutiBulletBatchNode->addChild(bulletLeft);
	this->m_pAllMutiBulletArr->addObject(bulletLeft);
	mutiBulletBatchNode->addChild(bulletRight);
	this->m_pAllMutiBulletArr->addObject(bulletRight);

	//bullet position
	Point planePos=PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getPosition();
	Point bulletLeftPos=Point(planePos.x-18,planePos.y+PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getContentSize().height/2);
	Point bulletRightPos=Point(planePos.x+18,planePos.y+PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getContentSize().height/2);
	bulletLeft->setPosition(bulletLeftPos);
	bulletRight->setPosition(bulletRightPos);

	//飞行距离
	float length=Director::getInstance()->getWinSize().height+bulletLeft->getContentSize().height/2-bulletLeftPos.y;
	float velocity=500/1 ;//320pixel/sec 速速
	float realMoveDuration=length/velocity; //时间

	FiniteTimeAction* L_actionMove=MoveTo::create(realMoveDuration,Point(bulletLeftPos.x,Director::getInstance()->getWinSize().height+bulletLeft->getContentSize().height/2));
	FiniteTimeAction* L_actionDone=CallFuncN::create(CC_CALLBACK_1(MutiBulletsLayer::mutiBulletMoveFinished,this));
	Sequence* L_sequence=Sequence::create(L_actionMove,L_actionDone,NULL);

	FiniteTimeAction* R_actionMOve=MoveTo::create(realMoveDuration,Point(bulletRightPos.x,Director::getInstance()->getWinSize().height+bulletRight->getContentSize().height/2));
	FiniteTimeAction* R_actionDone=CallFuncN::create(CC_CALLBACK_1(MutiBulletsLayer::mutiBulletMoveFinished,this));
	Sequence* R_sequence=Sequence::create(R_actionMOve,R_actionDone,NULL);

	bulletLeft->runAction(L_sequence);
	bulletRight->runAction(R_sequence);
}

void MutiBulletsLayer::mutiBulletMoveFinished(Object* pSender)
{
	Sprite* bullet=(Sprite*)pSender;
	this->mutiBulletBatchNode->removeChild(bullet,true);
	this->m_pAllMutiBulletArr->removeObject(bullet);
}

void MutiBulletsLayer::mutiBulletRemove(Sprite* bullet)
{
	if(bullet!=NULL)
	{
		this->mutiBulletBatchNode->removeChild(bullet,true);
		this->m_pAllMutiBulletArr->removeObject(bullet);
	}
}

void MutiBulletsLayer::startShoot(float delay)
{
	// 函数会被回调次数 30+1 ，delay第一次执行前的延时
	this->schedule(schedule_selector(MutiBulletsLayer::addMutiBullet),0.2f,30,delay);
}

void MutiBulletsLayer::stopShoot()
{
	this->unschedule(schedule_selector(MutiBulletsLayer::addMutiBullet));
}