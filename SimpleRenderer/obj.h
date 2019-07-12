#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "mesh.h"
using namespace std;

class Obj
{
public:
	static void Load(const string& filename, Mesh& mesh);
};

void Obj::Load(const string& filename, Mesh& mesh)
{
	ifstream in(filename.c_str());

	if (!in.good())
	{
		cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
		return;
	}

	char buffer[256];
	float v1, v2, v3;
	int vidx1, vidx2, vidx3, uvidx1, uvidx2, uvidx3, nidx1, nidx2, nidx3;
	int line = 1;

	while (!in.getline(buffer, 255).eof())
	{
		buffer[255] = '\0';

		//¶Á¶¥µã
		if (buffer[0] == 'v' && buffer[1] == ' ')
		{
			if (sscanf(buffer, "v %f %f %f", &v1, &v2, &v3) == 3)
			{
				mesh.vertices.push_back(Vector3(v1, v2, v3));
			}
			else
			{
				cout << "line " << line << " : vertex not in wanted format in OBJLoader" << "\n";
				return;
			}
		}
		//¶ÁÃæ
		else if (buffer[0] == 'f' && buffer[1] == ' ')
		{
			if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vidx1, &uvidx1, &nidx1, &vidx2, &uvidx2, &nidx2, &vidx3, &uvidx3, &nidx3) == 9)
			{
				mesh.triangles.push_back(vidx1);
				mesh.triangles.push_back(vidx2);
				mesh.triangles.push_back(vidx3);
			}
			else
			{
				cout << "line " << line << " : face not in wanted format in OBJLoader" << "\n";
				return;
			}
		}
		line++;
	}
}