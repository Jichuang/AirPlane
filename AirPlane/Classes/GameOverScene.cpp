#include "GameOverScene.h"

GameOverScene::GameOverScene():
	m_score(0),
	m_gameOverLayer(NULL)
{

}

GameOverScene::~GameOverScene()
{

}

GameOverScene* GameOverScene::create(int passScore)
{
	GameOverScene* pRect=new GameOverScene();
	pRect->m_score=passScore;
	if(pRect && pRect->init())
	{
		pRect->autorelease();
		return pRect;
	}
	else
	{
		pRect->release();
		pRect=NULL;
		return NULL;
	}
}

bool GameOverScene::init()
{
	if(!Scene::init())
	{
		return false;
	}

	m_gameOverLayer=GameOverLayer::create(m_score);
	this->addChild(m_gameOverLayer);
	return true;
}