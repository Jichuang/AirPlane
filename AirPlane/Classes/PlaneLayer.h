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

	static PlaneLayer* sharedPlane;//提供sharedPlane全局指针

	bool m_isAlive;
	int m_score;//最高20亿

	void moveTo(Point location);//飞机层移动方法 (触摸飞机移动)

	void blowup(int passScore);//飞机爆炸

	void removePlane();//移除飞机并转换至 GameOver场景
};

#endif //__PlaneLayer_H__