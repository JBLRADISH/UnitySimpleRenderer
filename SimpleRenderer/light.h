#pragma once

#include "color.h"
#include "transform.h"

enum LightType
{
	Directional,
	Point,
	Spot
};

class Light
{
public:
	Transform transform;
	LightType type;
	Color color;
	float intensity;
	//常量衰减因子
	float kc;
	//线性衰减因子
	float kl;
	//二次衰减因子
	float kq;
	//点光源范围
	float range;
	//聚光灯内锥角
	float spotInnerAngle;
	//聚光灯外锥角
	float spotOuterAngle;
	//聚光灯指数因子
	float pf;
};