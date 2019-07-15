#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "mesh.h"
using namespace std;

class Obj
{
public:
	static Mesh Load(const string& filename);
};

Mesh Obj::Load(const string& filename)
{
	Mesh mesh;
	ifstream in(filename.c_str());

	if (!in.good())
	{
		cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
		return mesh;
	}

	char buffer[256];
	float v1, v2, v3;
	int vidx1, vidx2, vidx3, uvidx1, uvidx2, uvidx3, nidx1, nidx2, nidx3;
	int line = 1;

	while (!in.getline(buffer, 255).eof())
	{
		buffer[255] = '\0';

		//读顶点
		if (buffer[0] == 'v' && buffer[1] == ' ')
		{
			if (sscanf(buffer, "v %f %f %f", &v1, &v2, &v3) == 3)
			{
				//unity加载obj模型会把x取反
				mesh.vertices.push_back(Vector3(-v1, v2, v3, 1.0f));
			}
			else
			{
				cout << "line " << line << " : vertex not in wanted format in OBJLoader" << "\n";
				return mesh;
			}
		}
		//读面
		else if (buffer[0] == 'f' && buffer[1] == ' ')
		{
			if (sscanf(buffer, "f %d %d %d", &vidx1, &vidx2, &vidx3) == 3)
			{
				mesh.faces.push_back(Face(vidx1 - 1, vidx2 - 1, vidx3 - 1));
			}
			else
			{
				cout << "line " << line << " : face not in wanted format in OBJLoader" << "\n";
				return mesh;
			}
		}
		line++;
	}
	return mesh;
}