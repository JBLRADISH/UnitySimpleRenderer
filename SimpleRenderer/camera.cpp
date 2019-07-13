#include "camera.h"

Camera::Camera(const Transform& transform, float fov, float near, float far, const Rect& viewport)
{
	this->transform = transform;
	this->fov = fov;
	this->near = near;
	this->far = far;
	this->viewport = viewport;
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
