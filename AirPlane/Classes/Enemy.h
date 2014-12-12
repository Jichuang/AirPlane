#ifndef __Enemy_H__
#define __Enemy_H__

#include "cocos2d.h"

USING_NS_CC;

class Enemy:public cocos2d::Node
{
public:
	Enemy();
    ~Enemy();

	CREATE_FUNC(Enemy);

	//�󶨵л�������������ֵ
	void bindSprite(Sprite* sprite,int life);

	//��ȡ�л���С
	Rect getBoundingBox();

	Sprite* getSprite();
	int getLife();
	void loseLife();
	void die();

	Sprite* m_sprite;
	int m_life;

};


#endif //__Enemy_H__