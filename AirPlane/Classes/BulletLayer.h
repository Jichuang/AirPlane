
/*子弹层*/
#ifndef __BulletLayer_H__
#define __BulletLayer_H__

#include "cocos2d.h"

USING_NS_CC;

class BulletLayer:public cocos2d::Layer
{
public:
	BulletLayer();
	~BulletLayer();

	virtual bool init();
	CREATE_FUNC(BulletLayer);

	void addBullet(float dt);

	SpriteBatchNode* bulletBatchNode; //加载子弹（）

	//子弹飞出屏幕
	void bulletMoveFuinished(Node* pSender);

	void startShoot(float delay=0.0f);
	void stopShoot();

	//删除某颗子弹(和敌机碰撞) 
	void removeBullet(Sprite* bullet);

	Array* m_pAllBullet;//创建成员变量指针(管理子弹)
};


#endif //__BulletLayer_H__

