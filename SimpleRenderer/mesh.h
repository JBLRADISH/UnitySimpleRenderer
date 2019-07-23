#pragma once

#include <vector>
#include "vector2.h"
#include "vector3.h"
#include "alignallocator.h"
#include "face.h"
#include "bounds.h"
using namespace std;

class Mesh
{
public:
	//SSE需要地址16字节对齐 所以要自定义Allocator
	vector<Vector3, AlignmentAllocator<Vector3>> vertices;
	vector<Face> faces;
	vector<Vector3, AlignmentAllocator<Vector3>> normals;
	vector<Vector2> uv;
	Bounds bounds;

	int vertexCount();

	void RecalculateNormals();
	void RecalculateBounds();

	Bounds GetWorldBounds(Matrix4x4& m);
};
