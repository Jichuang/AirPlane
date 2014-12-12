#include "WelcomeScene.h"

WelcomeScene::WelcomeScene():
	m_welcomeLayer(NULL)
{

}

WelcomeScene::~WelcomeScene()
{

}

bool WelcomeScene::init()
{
	if(!Scene::init())
	{
		return false;
	}

	//add WelcomeLayer
	m_welcomeLayer=WelcomeLayer::create();
	if(m_welcomeLayer==NULL)
	{
		return false;
	}
	this->addChild(m_welcomeLayer);

	preloadMusic();

	return true;
}

void WelcomeScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/game_music.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/bullet.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy1_down.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy2_down.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy3_down.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/game_over.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/get_bomb.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/get_double_laser.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/use_bomb.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/big_spaceship_flying.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/achievement.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/out_porp.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/button.wav");

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav",true);
}