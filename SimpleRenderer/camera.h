#pragma once

#include "rect.h"
#include "plane.h"
#include "transform.h"

class Camera
{
public:
	Transform transform;
	Rect viewport;
	float fov;
	float near;
	float far;

	Camera() = default;

	Camera(float fov, float near, float far, const Rect& viewport);

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
	Matrix4x4 projectionMatrix();
	Vector3 screenPoint(const Vector3& v);

private:
	float aspect;
};