#include "Enemy.h"

Enemy::Enemy():
	m_sprite(NULL),
	m_life(0)
{

}

Enemy::~Enemy()
{

}

void Enemy::bindSprite(Sprite* sprite,int life)
{
	m_sprite=sprite;
	m_life=life;
	this->addChild(m_sprite);
}

Sprite* Enemy::getSprite()
{
	return m_sprite;
}

int Enemy::getLife()
{
	return m_life;
}

void Enemy::loseLife()
{
	m_life--;
}

void Enemy::die()
{
	m_life=0;
}

//��ȡ�л���С
Rect Enemy::getBoundingBox()
{
	Rect rect=m_sprite->boundingBox();
	Point pos=this->convertToWorldSpace(rect.origin);//�ѻ��ڵ�ǰ�ڵ�ı�������ϵ�µ�����ת������������ϵ��
	Rect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);
	return enemyRect;
}