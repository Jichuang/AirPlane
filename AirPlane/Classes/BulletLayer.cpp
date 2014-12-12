#include "BulletLayer.h"
#include "PlaneLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

BulletLayer::BulletLayer():
	bulletBatchNode(NULL)
{
	/*��ΪArray::create ������autoRelease,�����ڵ��ô������ֶ�����retain
	���ⳬ��ʹ�÷�Χ���ͷ� ͬʱ�������������ٽ���release��ֹ�ڴ�й¶*/
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

	// ��Ҫ��Ⱦ�ľ�����ص�CCSpriteBatchNode (��Ⱦ�ӵ�) 
	Texture2D* texture=TextureCache::sharedTextureCache()->textureForKey("ui/shoot.png");
	bulletBatchNode=SpriteBatchNode::createWithTexture(texture);//bulletBatchNode ΪSpriteBatchNode ��Ա
	this->addChild(bulletBatchNode);

	return true;
}

void BulletLayer::addBullet(float dt)
{
	//mp3
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");

	Sprite* bullet=Sprite::createWithSpriteFrameName("bullet1.png");
	bulletBatchNode->addChild(bullet);//�ӵ�����ӵ� bulletBatchNode
	//this->addChild(bullet);//��ͨ��Ⱦ
	this->m_pAllBullet->addObject(bullet);

	/*�ɻ�������ҵĴ������ı�λ�ã��ӵ�λ���Ե�ǰ�ɻ���λ��Ϊ׼
	ͨ�� sharedPlane �� getChildByTag �õ��ɻ����飬Ȼ������ӵ��ĳ�ʼλ��*/
	Point planePosition=PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getPosition();
	Point bulletPosition=Point(planePosition.x,planePosition.y+PlaneLayer::sharedPlane->getChildByTag(Tag_Air_Plane)->getContentSize().height/2);
	bullet->setPosition(bulletPosition);

	//���о��룬������Ļ������
	float length=Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2- bulletPosition.y;
	float velocity=420/1; //�����ٶ�: 420 pixel/sec
	float realMoveDuration=length/velocity; //����ʱ��

	FiniteTimeAction* actionMove=MoveTo::create(realMoveDuration,Point(bulletPosition.x,Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2));
	FiniteTimeAction* actionDone=CCCallFuncN::create(this,callfuncN_selector(BulletLayer::bulletMoveFuinished));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	bullet->runAction(sequence);
}

void BulletLayer::bulletMoveFuinished(Node* pSender)
{
	Sprite* bullet=(Sprite*)pSender;
	this->m_pAllBullet->removeObject(bullet);//�Ƴ�Array
	this->bulletBatchNode->removeChild(bullet,true);//�Ƴ���Ļ
}

void BulletLayer::startShoot(float delay)
{
	// ʱ�� ����(n+1, kRepeatForever:����) ��ʱ(��һ�λص�ǰ)
	this->schedule(schedule_selector(BulletLayer::addBullet),0.2f,kRepeatForever,delay);
}

//ж������ִ����
void BulletLayer::stopShoot()
{
	this->unschedule(schedule_selector(BulletLayer::addBullet));
}

//ɾ��ĳ���ӵ� (һ���͵л���ײ����ɾ��)
void BulletLayer::removeBullet(Sprite* bullet)
{
	if(bullet!=NULL)
	{
		this->m_pAllBullet->removeObject(bullet);
		this->bulletBatchNode->removeChild(bullet,true);
	}
}