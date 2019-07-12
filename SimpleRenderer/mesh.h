#pragma once

#include <vector>
#include "vector3.h"
using namespace std;

class Mesh
{
public:
	vector<Vector3> vertices;
	vector<int> triangles;

	int vertexCount()
	{
		return vertices.size();
	}
};
