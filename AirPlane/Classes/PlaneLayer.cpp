#include "PlaneLayer.h"
#include "GameOverScene.h"

PlaneLayer* PlaneLayer::sharedPlane=NULL;

//自定义create
PlaneLayer* PlaneLayer::create()
{
	PlaneLayer* pRet=new PlaneLayer();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		sharedPlane=pRet;//获得静态指针sharedPlane的值
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

PlaneLayer::PlaneLayer():
	m_isAlive(true),
	m_score(0)
{

}

PlaneLayer::~PlaneLayer()
{

}

bool PlaneLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size winSize=Director::getInstance()->getWinSize();

	Sprite* plane=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero1.png"));
	plane->setPosition(Point(winSize.width/2,plane->getContentSize().height/2));
	this->addChild(plane,0,Tag_Air_Plane);//Tag_Air_Plane  是 tag

	Blink* blink=Blink::create(1,3);//闪速动画

	Animation* animation=Animation::create();
	animation->setDelayPerUnit(0.1f);//帧间隔时间
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero2.png"));
	Animate* animate=Animate::create(animation);

	plane->runAction(blink);
	plane->runAction(RepeatForever::create(animate));

	return true;
}

//触摸飞机移动
void PlaneLayer::moveTo(Point location)
{
	//飞机及游戏状态判定
	if(m_isAlive && !Director::getInstance()->isPaused()) //存活 && 非暂停（暂停方法二）
	{
		//进行边界判定，不可超出屏幕
		Point actualPoint;
		Size winSiz=Director::getInstance()->getWinSize();
		Size planeSize=this->getChildByTag(Tag_Air_Plane)->getContentSize();
		
		if(location.x<planeSize.width/2)
		{
			location.x=planeSize.width/2;
		}
		if(location.x>winSiz.width-planeSize.width/2)
		{
			location.x=winSiz.width-planeSize.width/2;
		}
		if(location.y<planeSize.height/2)
		{
			location.y=planeSize.height/2;
		}
		if(location.y>winSiz.height-planeSize.height/2)
		{
			location.y=winSiz.height-planeSize.height;
		}
		this->getChildByTag(Tag_Air_Plane)->setPosition(location);
	}
}

//飞机爆炸
void PlaneLayer::blowup(int passScore)
{
	if(m_isAlive)
	{
		m_isAlive=false;
		m_score=passScore;
		Animation* animation=Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n1.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n2.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n3.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n4.png"));

		Animate* animate=Animate::create(animation);
		CallFuncN* animateDone=CallFuncN::create(CC_CALLBACK_0(PlaneLayer::removePlane,this));
		Sequence* sequence=Sequence::create(animate,animateDone,NULL);

		this->getChildByTag(Tag_Air_Plane)->stopAllActions();
		this->getChildByTag(Tag_Air_Plane)->runAction(sequence);
	}
}

//移除飞机，跳转到GameOver场景
void PlaneLayer::removePlane()
{
	this->removeChildByTag(Tag_Air_Plane);
	GameOverScene* scene=GameOverScene::create(m_score);
	auto animationScene=TransitionFade::create(0.5f,scene);
	Director::getInstance()->replaceScene(animationScene);
}
