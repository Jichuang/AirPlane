#ifndef __UFOLayer_H__
#define __UFOLayer_H__

#include "cocos2d.h"

USING_NS_CC;

class UFOLayer: public cocos2d::Layer
{
public:
	UFOLayer();
	~UFOLayer();

	virtual bool init();

	CREATE_FUNC(UFOLayer);

	void addMutiBullets(float dt);//Ìí¼ÓË«ÅÅ×Óµ¯

	void mutiBulletsMoveFinished(Node* pSender);

	void addBigBoom(float dt);//Ìí¼ÓÕ¨µ¯

	void bigBoomMoveFinished(Node* pSender);

	void removeMutiBullets(Sprite* mutiBullets);

	void removeBigBoom(Sprite* bigBoom);

	Array* m_pAllMutiBullets;

	Array* m_pAllBigBoom;
};


#endif __UFOLayer_H__