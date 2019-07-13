#include "mathf.h"

float Clamp(float value, float min, float max)
{
	if ((double)value < (double)min)
		value = min;
	else if ((double)value > (double) max)
		value = max;
	return value;
}

bool Equal(float f1, float f2)
{
	if (fabsf(f2 - f1) <= Epsilon)
	{
		return true;
	}
	return false;
}