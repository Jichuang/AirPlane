#include "ControlLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

ControlLayer::ControlLayer()
{

}

ControlLayer::~ControlLayer()
{

}

bool ControlLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size winSize=Director::getInstance()->getWinSize();

	//加入PauseMenu
	m_pauseNormal=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_pause_nor.png"));
	m_pausePressed=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_pause_pressed.png"));
	
    m_pauseItem=MenuItemSprite::create(m_pauseNormal,m_pausePressed,NULL,CC_CALLBACK_1(ControlLayer::menuPauseCallback,this));//
	m_pauseItem->setPosition(Point(m_pauseNormal->getContentSize().width/2+10,winSize.height-m_pauseNormal->getContentSize().height/2-20));
	Menu* menuPause=Menu::create(m_pauseItem,NULL);
	menuPause->setPosition(Point::ZERO);
	this->addChild(menuPause,101);

	m_scoreItem=LabelBMFont::create("0","font/font.fnt");
	m_scoreItem->setColor(Color3B(143,146,147));
	m_scoreItem->setAnchorPoint(Point(0,0.5));
	m_scoreItem->setPosition(Point(m_pauseItem->getPositionX()+m_pauseItem->getContentSize().width/2+5,m_pauseItem->getPositionY()));
	this->addChild(m_scoreItem);

	return true;
}

void ControlLayer::menuPauseCallback(Object* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");

	if(!Director::getInstance()->isPaused())//如果游戏处于正常状态
	{
		//更改为恢复按钮的状态
		m_pauseItem->setNormalImage(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_resume_nor.png")));
		m_pauseItem->setSelectedImage(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_resume_pressed.png")));
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->pauseAllEffects();
		Director::getInstance()->pause();//游戏暂停
		//no touch  (在 PlaneLayer 里判断)
		//m_noTouchLayer=NoTouchLayer::create();
		//this->addChild(m_noTouchLayer);
	}
	else
	{
		//更改为按钮暂停的状态
		m_pauseItem->setNormalImage(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_pause_nor.png")));//选中
		m_pauseItem->setSelectedImage(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("game_pause_pressed.png")));//不选中
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		Director::getInstance()->resume();//恢复游戏
		//resume touch
		//this->removeChild(m_noTouchLayer,true);
	}
}

void ControlLayer::updateScore(int score)
{
	if(score>=0 && score<=MAX_SCORE)
	{
		m_scoreItem->setString(String::createWithFormat("%d",score)->getCString());
	}
}