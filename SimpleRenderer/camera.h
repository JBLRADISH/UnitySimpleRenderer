#pragma once

#include "rect.h"
#include "plane.h"
#include "transform.h"
#include "mesh.h"

class Camera
{
public:
	Transform transform;
	Rect viewport;

	Camera() = default;
	Camera(float fov, float zNear, float zFar, const Rect& viewport);

	float GetFov() { return fov; }
	float GetZNear() { return zNear; }
	float GetZfar() { return zFar; }
	void SetFov(float fov) { this->fov = fov; pDirty = true; vpDirty = true; frustumDirty = true; }
	void SetZNear(float zNear) { this->zNear = zNear; pDirty = true; vpDirty = true;  frustumDirty = true; }
	void SetZFar(float zFar) { this->zFar = zFar; pDirty = true; vpDirty = true; frustumDirty = true; }

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4& worldToCameraMatrix();
	Matrix4x4& projectionMatrix();
	Matrix4x4& viewProjectionMatrix();

	void SetClipPlane();
	bool OutSide(const Bounds& bounds);

	bool CullFace_WorldSpace(Vector3& v1, Vector3& v2, Vector3& v3);

private:
	float fov;
	float zNear;
	float zFar;
	Matrix4x4 worldToCamera;
	Matrix4x4 projection;
	Matrix4x4 viewProjection;
	bool pDirty;
	bool vpDirty;
	bool frustumDirty;
	float aspect;
	Plane planes[6];
};