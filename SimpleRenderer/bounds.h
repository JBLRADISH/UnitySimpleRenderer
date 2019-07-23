#pragma once

#include "vector3.h"
#include "matrix4x4.h"
#include <vector>
using namespace std;

class Bounds
{
public:

	Vector3 min;
	Vector3 max;

	void RecalculateBounds(const vector<Vector3, AlignmentAllocator<Vector3>>& vertices)
	{
		min = max = vertices[0];
		for (int i = 1; i < vertices.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vector3 cur = vertices[i];
				if (cur[j] < min[j])
					min[j] = cur[j];
				if (cur[j] > max[j])
					max[j] = cur[j];
			}
		}
	}

	Bounds GetWorldBounds(Matrix4x4& m)
	{
		Bounds res;
		Vector3 translate(m.m03, m.m13, m.m23);
		res.min = translate;
		res.max = translate;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float e = m.GetRow(j)[i] * min[i];
				float f = m.GetRow(j)[i] * max[i];
				if (e < f)
				{
					res.min[j] += e;
					res.max[j] += f;
				}
				else
				{
					res.min[j] += f;
					res.max[j] += e;
				}
			}
		}
		return res;
	}
};