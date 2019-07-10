#pragma once

const float Deg2Rad = 0.01745329f;

const float Rad2Deg = 57.29578f;

float Clamp(float value, float min, float max)
{
	if ((double)value < (double)min)
		value = min;
	else if ((double)value > (double) max)
		value = max;
	return value;
}

