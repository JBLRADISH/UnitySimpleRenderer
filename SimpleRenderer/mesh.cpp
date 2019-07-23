#include "mesh.h"

int Mesh::vertexCount()
{
	return vertices.size();
}

void Mesh::RecalculateNormals()
{
	for (int i = 0; i < vertexCount(); i++)
	{
		normals.push_back(Vector3::zero);
	}
	for (int i = 0; i < faces.size(); i++)
	{
		int vidx1 = faces[i].vidx1;
		int vidx2 = faces[i].vidx2;
		int vidx3 = faces[i].vidx3;
		Vector3 v1 = vertices[vidx1];
		Vector3 v2 = vertices[vidx2];
		Vector3 v3 = vertices[vidx3];
		Vector3 e0 = v1 - v2;
		Vector3 e1 = v2 - v3;
		Vector3 n = Vector3::Cross(e1, e0);
		normals[vidx1] = normals[vidx1] + n;
		normals[vidx2] = normals[vidx2] + n;
		normals[vidx3] = normals[vidx3] + n;
	}
	for (int i = 0; i < vertexCount(); i++)
	{
		normals[i] = normals[i].normalized();
	}
}

void Mesh::RecalculateBounds()
{
	bounds.RecalculateBounds(vertices);
}

Bounds Mesh::GetWorldBounds(Matrix4x4& m)
{
	return bounds.GetWorldBounds(m);
}