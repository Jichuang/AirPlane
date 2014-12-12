/*双排子弹，实现跟单排的一样一样*/

#ifndef __MutiBulletsLayer_H__
#define __MutiBulletsLayer_H__

#include "cocos2d.h"
#include "PlaneLayer.h"
USING_NS_CC;

class MutiBulletsLayer:public cocos2d::Layer
{
public:
	MutiBulletsLayer();
	~MutiBulletsLayer();

	virtual bool init();

	CREATE_FUNC(MutiBulletsLayer);

	void startShoot(float delay=0);
	void stopShoot();

	void addMutiBullet(float dt);

	void mutiBulletMoveFinished(Object* Psender);

	void mutiBulletRemove(Sprite* bullet);

	SpriteBatchNode* mutiBulletBatchNode;
	Array* m_pAllMutiBulletArr;

};

#endif //__MutiBUlletsLayer_H__