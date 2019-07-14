#include "camera.h"

Camera::Camera(float fov, float near, float far, const Rect& viewport)
{
	this->fov = fov;
	this->near = near;
	this->far = far;
	this->viewport = viewport;
	aspect = (float)viewport.width / (float)viewport.height;
}

Matrix4x4 Camera::cameraToWorldMatrix()
{
	return transform.localToWorldMatrix();
}

Matrix4x4 Camera::worldToCameraMatrix()
{
	return transform.worldToLocalMatrix();
}

Matrix4x4 Camera::projectionMatrix()
{
	return Matrix4x4::Perspective(fov, aspect, near, far);
}

Vector3 Camera::screenPoint(const Vector3& v)
{
	float ndcX = v.x / v.w;
	float ndcY = v.y / v.w;
	Vector3 screenPos;
	screenPos.x = viewport.x + (ndcX + 1.0f) * 0.5f * viewport.width;
	screenPos.y = viewport.y + (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;
	return screenPos;
}
