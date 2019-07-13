#pragma once

#include <math.h>

const float Deg2Rad = 0.01745329f;

const float Rad2Deg = 57.29578f;

const float Tolerance = 2e-37f;

const float Epsilon = 1.401298E-45f;

float Clamp(float value, float min, float max);

bool Equal(float f1, float f2);

