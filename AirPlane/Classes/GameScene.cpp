#include "GameScene.h"

GameScene::GameScene():
	m_gameLayer(NULL)
{

}
GameScene::~GameScene()
{

}

bool GameScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	m_gameLayer=GameLayer::create();
	this->addChild(m_gameLayer,0);

	//background
	background1=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
	background1->getTexture()->setAntiAliasTexParameters();//防止叠加处的黑边
	background1->setAnchorPoint(Point(0,0));
	background1->setPosition(Point(0,0));
	this->addChild(background1,-1);

	background2=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
	background2->getTexture()->setAntiAliasTexParameters();//防止叠加处的黑边
	background2->setAnchorPoint(Point(0,0));
	background2->setPosition(Point(0,background2->getContentSize().height-2.0f));//减2是为了防止移动时的黑线
	this->addChild(background2,-1);

	//地图移动 (自定义update 函数)
	this->schedule(schedule_selector(GameScene::backgroundMove),0.01f);

	return true;
}

//地图滚动
void GameScene::backgroundMove(float dt)
{
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY((background1->getPositionY()+background2->getContentSize().height)-2);

	/*背景图高度842 每次减2可达0   (屏幕：480,800)： 注意842>800 才不会出现空白*/
	if(background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}