#ifndef __PlaneLayer_H__
#define __PlaneLayer_H__

#include "cocos2d.h"

const int Tag_Air_Plane=747;

USING_NS_CC;

class PlaneLayer:public cocos2d::Layer
{
public:
	PlaneLayer();
    ~PlaneLayer();

	static PlaneLayer* create();
	virtual bool init();

	static PlaneLayer* sharedPlane;//�ṩsharedPlaneȫ��ָ��

	bool m_isAlive;
	int m_score;//���20��

	void moveTo(Point location);//�ɻ����ƶ����� (�����ɻ��ƶ�)

	void blowup(int passScore);//�ɻ���ը

	void removePlane();//�Ƴ��ɻ���ת���� GameOver����
};

#endif //__PlaneLayer_H__