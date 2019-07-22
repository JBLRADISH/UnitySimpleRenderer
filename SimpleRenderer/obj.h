#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "gameobject.h"
using namespace std;

class Obj
{
public:
	static GameObject Load(const string& filename);
	static Material LoadMaterial(const string& filename);
};

GameObject Obj::Load(const string& filename)
{
	GameObject res;
	ifstream in(filename.c_str());

	if (!in.good())
	{
		cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
		return res;
	}

	char buffer[256];
	float v1, v2, v3;
	int vidx1, vidx2, vidx3, uvidx1, uvidx2, uvidx3;
	float uv1, uv2, uv3;
	int line = 1;

	while (!in.getline(buffer, 255).eof())
	{
		string str(buffer);
		size_t idx = str.find(' ');
		string tag = "";
		if (idx != string::npos)
			tag = str.substr(0, idx);
		//读顶点
		if (tag == "v")
		{
			if (sscanf(buffer, "v %f %f %f", &v1, &v2, &v3) == 3)
			{
				//unity加载obj模型会把x取反
				res.mesh.vertices.push_back(Vector3(-v1, v2, v3, 1.0f));
			}
			else
			{
				cout << "line " << line << " : vertex not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读面
		else if (tag == "f")
		{
			if (sscanf(buffer, "f %d/%d %d/%d %d/%d", &vidx1, &uvidx1, &vidx2, &uvidx2, &vidx3, &uvidx3) == 6)
			{
				res.mesh.faces.push_back(Face(vidx1 - 1, vidx2 - 1, vidx3 - 1));
			}
			else if (sscanf(buffer, "f %d %d %d", &vidx1, &vidx2, &vidx3) == 3)
			{
				res.mesh.faces.push_back(Face(vidx1 - 1, vidx2 - 1, vidx3 - 1));
			}
			else
			{
				cout << "line " << line << " : face not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读UV
		else if (tag == "vt")
		{
			if (sscanf(buffer, "vt %f %f %f", &uv1, &uv2, &uv3) == 3)
			{
				res.mesh.uv.push_back(Vector2(uv1, uv2));
			}
			else
			{
				cout << "line " << line << " : uv not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读材质库
		else if (tag == "mtllib")
		{
			string material = str.substr(idx + 1);
			res.material = LoadMaterial(material);
		}
		line++;
	}

	//计算顶点法线
	res.mesh.RecalculateNormals();

	return res;
}

Material Obj::LoadMaterial(const string& filename)
{
	Material res;
	ifstream in(filename.c_str());

	if (!in.good())
	{
		cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
		return res;
	}

	char buffer[256];
	float r, g, b;
	float power;
	int line = 1;

	while (!in.getline(buffer, 255).eof())
	{
		string str(buffer);
		size_t idx = str.find(' ');
		string tag = "";
		if (idx != string::npos)
			tag = str.substr(0, idx);
		//读环境光颜色
		if (tag == "Ka")
		{
			if (sscanf(buffer, "Ka %f %f %f", &r, &g, &b) == 3)
			{
				res.cAmbient = Color(r, g, b);
			}
			else
			{
				cout << "line " << line << " : Ka not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读漫反射颜色
		else if (tag == "Kd")
		{
			if (sscanf(buffer, "Kd %f %f %f", &r, &g, &b) == 3)
			{
				res.cDiffuse = Color(r, g, b);
			}
			else
			{
				cout << "line " << line << " : Kd not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读高光反射颜色
		else if (tag == "Ks")
		{
			if (sscanf(buffer, "Ks %f %f %f", &r, &g, &b) == 3)
			{
				res.cSpecular = Color(r, g, b);
			}
			else
			{
				cout << "line " << line << " : Ks not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		//读高光反射指数
		else if (tag == "Ns")
		{
			if (sscanf(buffer, "Ns %f", &power) == 1)
			{
				res.power = power;
			}
			else
			{
				cout << "line " << line << " : Ns not in wanted format in OBJLoader" << "\n";
				return res;
			}
		}
		line++;
	}
	return res;
}