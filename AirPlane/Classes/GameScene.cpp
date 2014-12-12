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
	background1->getTexture()->setAntiAliasTexParameters();//��ֹ���Ӵ��ĺڱ�
	background1->setAnchorPoint(Point(0,0));
	background1->setPosition(Point(0,0));
	this->addChild(background1,-1);

	background2=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
	background2->getTexture()->setAntiAliasTexParameters();//��ֹ���Ӵ��ĺڱ�
	background2->setAnchorPoint(Point(0,0));
	background2->setPosition(Point(0,background2->getContentSize().height-2.0f));//��2��Ϊ�˷�ֹ�ƶ�ʱ�ĺ���
	this->addChild(background2,-1);

	//��ͼ�ƶ� (�Զ���update ����)
	this->schedule(schedule_selector(GameScene::backgroundMove),0.01f);

	return true;
}

//��ͼ����
void GameScene::backgroundMove(float dt)
{
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY((background1->getPositionY()+background2->getContentSize().height)-2);

	/*����ͼ�߶�842 ÿ�μ�2�ɴ�0   (��Ļ��480,800)�� ע��842>800 �Ų�����ֿհ�*/
	if(background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}