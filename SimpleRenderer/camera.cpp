#include "camera.h"

Camera::Camera(float fov, float zNear, float zFar, const Rect& viewport)
{
	this->fov = fov;
	this->zNear = zNear;
	this->zFar = zFar;
	this->viewport = viewport;
	aspect = (float)viewport.width / (float)viewport.height;
}

Matrix4x4 Camera::cameraToWorldMatrix()
{
	Matrix4x4 invView = transform.localToWorldMatrix();
	Matrix4x4 invNegatez = Matrix4x4::identity;
	invNegatez[2][2] = -1;
	return invView * invNegatez;
}

Matrix4x4 Camera::worldToCameraMatrix()
{
	Matrix4x4 view = transform.worldToLocalMatrix();
	Matrix4x4 negatez = Matrix4x4::identity;
	negatez[2][2] = -1;
	return negatez * view;
}

Matrix4x4 Camera::projectionMatrix()
{
	return Matrix4x4::Perspective(fov, aspect, zNear, zFar);
}

//基于世界空间背面裁剪
bool Camera::CullFace_WorldSpace(Vector3& v1, Vector3& v2, Vector3& v3)
{
	Vector3 e0 = v1 - v2;
	Vector3 e1 = v2 - v3;
	Vector3 n = Vector3::Cross(e1, e0);
	Vector3 view = transform.position - v1;
	float dp = Vector3::Dot(n, view);
	if (dp <= 0.0f)
	{
		return false;
	}
	return true;
}