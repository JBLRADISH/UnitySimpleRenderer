#pragma once

#include <vector>
#include "vector3.h"
#include "alignallocator.h"
#include "face.h"
using namespace std;

class Mesh
{
public:
	//SSE需要地址16字节对齐 所以要自定义Allocator
	vector<Vector3, AlignmentAllocator<Vector3>> vertices;
	vector<Face> faces;

	int vertexCount()
	{
		return vertices.size();
	}
};
