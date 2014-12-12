#include "WelcomeLayer.h"
#include "GameScene.h"
#include "GameOverLayer.h"

bool WelcomeLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size winSize=Director::getInstance()->getWinSize();

	//在Welcome中加载：全局变量 
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot.plist");
	//png 加到全局cache 中
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot_background.plist");

	//background
	Sprite* background=Sprite::createWithSpriteFrameName("background.png");
	background->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(background);

	//copyright
	Sprite* copyRight=Sprite::create("ui/copyRight.png");
	copyRight->setPosition(Point(winSize.width/2,winSize.height/2+100));
	this->addChild(copyRight);

	//plane
	Sprite* loading=Sprite::createWithSpriteFrameName("game_loading1.png");
	loading->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(loading);

	//animation
	Animation* animation=Animation::create();
	animation->setDelayPerUnit(0.3);
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_loading1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_loading2.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_loading3.png"));
    animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_loading4.png"));

	Animate* animate=Animate::create(animation);
	RepeatForever* repeat=RepeatForever::create(animate);

	loading->runAction(repeat);

	//start menu
	auto itme=MenuItemLabel::create(LabelTTF::create("Start","Arial",60),
		this,
		menu_selector(WelcomeLayer::menuCloseCallback));
	itme->setColor(Color3B(143,146,147));
	itme->setPosition(Point(winSize.width/2,winSize.height/2-100));
	Menu* menu=Menu::create(itme,NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//m_highestHistoryScore 获取历史最高分
	getHighesHistoryScore();

	return true;
}

void WelcomeLayer::menuCloseCallback(Object* pSender)
{
	Scene* scene=GameScene::create();
	Director::getInstance()->replaceScene(scene);
}

//判断存档是否存在
bool WelcomeLayer::isHaveSaveFile()
{
	if(!UserDefault::getInstance()->getBoolForKey("isHaveSaveFileXml"))
	{
		UserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml",true);//写入bool判断位
		UserDefault::getInstance()->setIntegerForKey("HighestScore",0);//写入初始分数0
		UserDefault::getInstance()->flush();//设置完一定要调用flush ，才能从缓冲写入io
		return false;
	}
	else
	{
		return true;
	}
}

void WelcomeLayer::getHighesHistoryScore()
{
	if(isHaveSaveFile()) //如果存档存在
	{
		//读取历史最高分
		GameOverLayer::m_highestHistoryScore=UserDefault::getInstance()->getIntegerForKey("HighestScore",0);
	}
}