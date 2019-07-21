#pragma once

#include <math.h>

const float Deg2Rad = 0.01745329f;

const float Rad2Deg = 57.29578f;

const float Tolerance = 2e-37f;

const float Epsilon = 1.401298E-45f;

float Clamp(float value, float min, float max);

int Clamp(int value, int min, int max);

bool Equal(float f1, float f2);

float SmoothStep(float t);

float Lerp(float f1, float f2, float t);

inline int Round(float value)
{
	int res;
	__asm
	{
		fld value;
		fistp res;
	}
	return res;
}

inline int Floor(float value)
{
	float tmp = value - 0.5f;
	int res;
	__asm
	{
		fld tmp;
		fistp res;
	}
	return res;
}

inline int Ceil(float value)
{
	float tmp = value + 0.49999999f;
	int res;
	__asm
	{
		fld tmp;
		fistp res;
	}
	return res;
}