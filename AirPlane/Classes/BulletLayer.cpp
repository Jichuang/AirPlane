#include "BulletLayer.h"
#include "PlaneLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

BulletLayer::BulletLayer():
	bulletBatchNode(NULL)
{
	/*因为Array::create 调用了autoRelease,所以在调用创建后手动进行retain
	以免超出使用范围后被释放 同时在析构函数中再进行release防止内存泄露*/
	m_pAllBullet=Array::create();
	m_pAllBullet->retain();
}

BulletLayer::~BulletLayer()
{
	m_pAllBullet->release();
	m_pAllBullet=NULL;
}

bool BulletLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	// 将要渲染的精灵加载到CCSpriteBatchNode (渲染子弹) 
	Texture2D* texture=TextureCache::sharedTextureCache()->textureForKey("ui/shoot.png");
	bulletBatchNode=SpriteBatchNode::createWithTexture(texture);//bulletBatchNode 为SpriteBatchNode 成员
	this->addChild(bulletBatchNode);

	return true;
}

void BulletLayer::addBullet(float dt)
{
	//mp3
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");

	Sprite* bullet=Sprite::createWithSpriteFrameName("bullet1.png");
	bulletBatchNode->addChild(bullet);//子弹层添加到 bulletBatchNode
	//this->addChild(bullet);//普通渲染
	this->m_pAllBullet->addObject(bullet);

	/*飞机随着玩家的触摸而改变位置：子弹位置以当前飞机的位置为准
	通过 sharedPlane 的 getChildByTag 得到飞机精灵，然后计算子弹的初始位置*/
	Point planePosition=PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getPosition();
	Point bulletPosition=Point(planePosition.x,planePosition.y+PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getContentSize().height/2);
	bullet->setPosition(bulletPosition);

	//飞行距离，超出屏幕即结束
	float length=Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2- bulletPosition.y;
	float velocity=420/1; //飞行速度: 420 pixel/sec
	float realMoveDuration=length/velocity; //飞行时间

	FiniteTimeAction* actionMove=MoveTo::create(realMoveDuration,Point(bulletPosition.x,Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2));
	FiniteTimeAction* actionDone=CCCallFuncN::create(this,callfuncN_selector(BulletLayer::bulletMoveFuinished));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	bullet->runAction(sequence);
}

void BulletLayer::bulletMoveFuinished(Node* pSender)
{
	Sprite* bullet=(Sprite*)pSender;
	this->m_pAllBullet->removeObject(bullet);//移除Array
	this->bulletBatchNode->removeChild(bullet,true);//移除屏幕
}

void BulletLayer::startShoot(float delay)
{
	// 时间 次数(n+1, kRepeatForever:无限) 延时(第一次回调前)
	this->schedule(schedule_selector(BulletLayer::addBullet),0.2f,kRepeatForever,delay);
}

//卸载任务执行器
void BulletLayer::stopShoot()
{
	this->unschedule(schedule_selector(BulletLayer::addBullet));
}

//删除某颗子弹 (一旦和敌机碰撞，就删除)
void BulletLayer::removeBullet(Sprite* bullet)
{
	if(bullet!=NULL)
	{
		this->m_pAllBullet->removeObject(bullet);
		this->bulletBatchNode->removeChild(bullet,true);
	}
}