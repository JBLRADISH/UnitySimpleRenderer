#include "camera.h"

Camera::Camera(float fov, float zNear, float zFar, const Rect& viewport)
{
	this->fov = fov;
	this->zNear = zNear;
	this->zFar = zFar;
	this->viewport = viewport;
	aspect = (float)viewport.width / (float)viewport.height;
	pDirty = true;
	vpDirty = true;
	frustumDirty = true;
}

Matrix4x4 Camera::cameraToWorldMatrix()
{
	Matrix4x4 invView = transform.localToWorldMatrix();
	Matrix4x4 invNegatez = Matrix4x4::identity;
	invNegatez[2][2] = -1;
	return invView * invNegatez;
}

Matrix4x4& Camera::worldToCameraMatrix()
{
	const Matrix4x4& view = transform.worldToLocalMatrix();
	Matrix4x4 negatez = Matrix4x4::identity;
	negatez[2][2] = -1;
	Matrix4x4 res = negatez * view;
	if (memcmp(res.m, worldToCamera.m, sizeof(float) * 16) != 0)
	{
		worldToCamera = res;
		vpDirty = true;
		frustumDirty = true;
	}
	return worldToCamera;
}

Matrix4x4& Camera::projectionMatrix()
{
	if (pDirty)
	{
		projection = Matrix4x4::Perspective(fov, aspect, zNear, zFar);
		pDirty = false;
		vpDirty = true;
		frustumDirty = true;
	}
	return projection;
}

Matrix4x4& Camera::viewProjectionMatrix()
{
	if (vpDirty)
	{
		viewProjection = projectionMatrix() * worldToCameraMatrix();
		vpDirty = false;
	}
	return viewProjection;
}

void Camera::SetClipPlane()
{
	if (frustumDirty)
	{
		const Matrix4x4& m = viewProjectionMatrix();
		planes[0] = Plane(Vector3(m.m[3] + m.m[0], m.m[7] + m.m[4], m.m[11] + m.m[8]) * -1.0f, (m.m[15] + m.m[12]));//left
		planes[1] = Plane(Vector3(m.m[3] - m.m[0], m.m[7] - m.m[4], m.m[11] - m.m[8]) * -1.0f, (m.m[15] - m.m[12]));//right
		planes[2] = Plane(Vector3(m.m[3] + m.m[1], m.m[7] + m.m[5], m.m[11] + m.m[9]) * -1.0f, (m.m[15] + m.m[13]));//bottom
		planes[3] = Plane(Vector3(m.m[3] - m.m[1], m.m[7] - m.m[5], m.m[11] - m.m[9]) * -1.0f, (m.m[15] - m.m[13]));//top
		planes[4] = Plane(Vector3(m.m[3] + m.m[2], m.m[7] + m.m[6], m.m[11] + m.m[10]) * -1.0f, (m.m[15] + m.m[14]));//near
		planes[5] = Plane(Vector3(m.m[3] - m.m[2], m.m[7] - m.m[6], m.m[11] - m.m[10]) * -1.0f, (m.m[15] - m.m[14]));//far
		frustumDirty = false;
	}
}

bool Camera::OutSide(const Bounds& bounds)
{
	SetClipPlane();
	Vector3 v;
	for (int i = 0; i < 6; i++)
	{
		const Vector3& normal = planes[i].n;
		v.x = normal.x < 0 ? bounds.max.x : bounds.min.x;
		v.y = normal.y < 0 ? bounds.max.y : bounds.min.y;
		v.z = normal.z < 0 ? bounds.max.z : bounds.min.z;
		if (!planes[i].Inside(v))
		{
			return true;
		}
	}
	return false;
}

//基于世界空间背面裁剪
bool Camera::CullFace_WorldSpace(Vector3& v1, Vector3& v2, Vector3& v3)
{
	Vector3 e0 = v1 - v2;
	Vector3 e1 = v2 - v3;
	Vector3 n = Vector3::Cross(e1, e0);
	Vector3 view = transform.GetPosition() - v1;
	float dp = Vector3::Dot(n, view);
	if (dp <= 0.0f)
	{
		return false;
	}
	return true;
}