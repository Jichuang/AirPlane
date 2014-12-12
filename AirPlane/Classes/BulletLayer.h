
/*�ӵ���*/
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

	SpriteBatchNode* bulletBatchNode; //�����ӵ�����

	//�ӵ��ɳ���Ļ
	void bulletMoveFuinished(Node* pSender);

	void startShoot(float delay=0.0f);
	void stopShoot();

	//ɾ��ĳ���ӵ�(�͵л���ײ) 
	void removeBullet(Sprite* bullet);

	Array* m_pAllBullet;//������Ա����ָ��(�����ӵ�)
};


#endif //__BulletLayer_H__

