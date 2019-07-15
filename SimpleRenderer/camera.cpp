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
	return Matrix4x4::Perspective(fov, aspect, near, far);
}

Vector3 Camera::screenPoint(const Vector3& v)
{
	float invW = 1 / v.w;
	float ndcX = v.x * invW;
	float ndcY = v.y * invW;
	Vector3 screenPos;
	screenPos.x = viewport.x + (ndcX + 1.0f) * 0.5f * (viewport.width - 1);
	screenPos.y = viewport.y + (1.0f - (ndcY + 1.0f) * 0.5f) * (viewport.height - 1);
	return screenPos;
}

//在世界空间下进行背面裁剪
bool Camera::CullFace(Vector3& v1, Vector3& v2, Vector3& v3)
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
