#include "GameOverLayer.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

class GameOverScene;

int GameOverLayer::m_highestHistoryScore=0;

GameOverLayer::GameOverLayer():
	m_score(0),
	m_highestScore(NULL)
{

}

GameOverLayer::~GameOverLayer()
{

}

GameOverLayer* GameOverLayer::create(int passScore)
{
	GameOverLayer* pRet=new GameOverLayer();
	pRet->m_score=passScore;
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		pRet->release();
		pRet=NULL;
		return NULL;
	}
}

bool GameOverLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	SimpleAudioEngine::getInstance()->playEffect("sound/game_over.wav");

	Size winSize=Director::getInstance()->getWinSize();

	Sprite* background=Sprite::createWithSpriteFrameName("gameover.png");
	background->setScaleY(0.95);
	background->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(background);

	//back to game menu
	Sprite* normalBackToGame=Sprite::createWithSpriteFrameName("btn_finish.png");
	Sprite* pressBackToGame=Sprite::createWithSpriteFrameName("btn_finish.png");
	MenuItemSprite* menuitem=MenuItemSprite::create(normalBackToGame,pressBackToGame,NULL,CC_CALLBACK_1(GameOverLayer::menuBackCallback,this));
	menuitem->setPosition(Point(winSize.width/2,winSize.height/2-180));
	Menu* menu=Menu::create(menuitem,NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//exit menu
	auto item=MenuItemLabel::create(LabelTTF::create("Exit","Arial",60),
		this,
		menu_selector(GameOverLayer::menuExit));
	item->setColor(Color3B(143,146,147));
	item->setPosition(Point(winSize.width-75,30));
	Menu* menu2=Menu::create(item,NULL);
	menu2->setPosition(Point::ZERO);
	this->addChild(menu2,5);

	//final score
	String* strScore=String::createWithFormat("%d",m_score);
	LabelBMFont* finalScore=LabelBMFont::create(strScore->getCString(),"font/font.fnt");
	finalScore->setColor(Color3B(143,146,147));
	finalScore->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(finalScore);

	DelayTime* delay=DelayTime::create(1.0f);
	ScaleTo* scaleBig=ScaleTo::create(1.0f,3.0f);
	ScaleTo* scaleSmall=ScaleTo::create(0.3f,2.0f);
	CallFunc* showAD=CallFunc::create(CC_CALLBACK_0(GameOverLayer::showAD,this));
	Sequence* sequence=Sequence::create(delay,scaleBig,scaleSmall,showAD,NULL);

	finalScore->runAction(sequence);

	//highest score
	String* strHighestScore=String::createWithFormat("%d",m_highestHistoryScore);
	m_highestScore=LabelBMFont::create(strHighestScore->getCString(),"font/font.fnt");
	m_highestScore->setColor(Color3B(143,146,147));
	m_highestScore->setAnchorPoint(Point(0,0.5));
	m_highestScore->setPosition(Point(140,winSize.height-50));
	this->addChild(m_highestScore);

	if(m_score > m_highestHistoryScore)
	{
		UserDefault::getInstance()->setIntegerForKey("HighestScore",m_score);
		m_highestHistoryScore=m_score;
		DelayTime* delayChange=DelayTime::create(1.3f);
		MoveBy* moveOut=MoveBy::create(0.1f,Point(0,100));
		CallFuncN* beginChange=CallFuncN::create(CC_CALLBACK_1(GameOverLayer::beginChangeHighestScore,this));
		MoveBy* moveIn=MoveBy::create(0.1f,Point(0,-100));
		Sequence* seq=Sequence::create(delayChange,moveOut,beginChange,moveIn,NULL);
		m_highestScore->runAction(seq);
	}

	this->setKeyboardEnabled(true);

	return true;
}

void GameOverLayer::menuBackCallback(Object* pSender)
{
	GameScene* scene=GameScene::create();
	auto animationScene=TransitionCrossFade::create(0.5f,scene);
	Director::getInstance()->replaceScene(animationScene);
}

void GameOverLayer::beginChangeHighestScore(Node* pNode)
{

	SimpleAudioEngine::getInstance()->playEffect("sound/achievement.wav");
	String* changeScore=String::createWithFormat("%d",m_score);
	m_highestScore->setString(changeScore->getCString());
}

void GameOverLayer::showAD()
{

}

void GameOverLayer::menuExit(Object* obj)
{
	Director::getInstance()->end();
}