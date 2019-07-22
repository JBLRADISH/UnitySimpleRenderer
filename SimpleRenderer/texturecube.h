#pragma once

#include "texture2D.h"
#include <vector>

class TextureCube :public Texture {
public:
	void Load(vector<string> filename, bool mipmap)
	{
		for (int i = 0; i < filename.size(); i++)
		{
			Texture2D* tex = new Texture2D;
			tex->Load(filename[i], mipmap);
			texs.push_back(tex);
		}
	}

	void UnLoad()
	{
		for (int i = 0; i < texs.size(); i++)
		{
			texs[i]->UnLoad();
			delete texs[i];
			texs[i] = NULL;
		}
	}

	Color GetColor(const Vector3& dir) const
	{
		int face = GetFace(dir);
		return texs[face]->GetColor(GetUV(dir, face));
	}

private:
	vector<Texture2D*> texs;

	int GetFace(const Vector3& dir) const
	{
		Vector3 tmp = dir;
		int max = 0;
		for (int i = 1; i < 3; i++)
		{
			if (fabsf(tmp[i]) > fabsf(tmp[max]))
			{
				max = i;
			}
		}
		return max + (tmp[max] < 0 ? 3 : 0);
	}

	Vector3 GetUV(const Vector3& dir, int face) const
	{
		Vector3 tmp = dir;
		float u = 0, v = 0;
		switch (face)
		{
		case 0:
			u = 1.0f - tmp[2];
			v = 1.0f - tmp[1];
			break;
		case 1:
			u = 1.0f + tmp[0];
			v = 1.0f + tmp[2];
			break;
		case 2:
			u = 1.0f + tmp[0];
			v = 1.0f - tmp[1];
			break;
		case 3:
			u = 1.0f + tmp[2];
			v = 1.0f - tmp[1];
			break;
		case 4:
			u = 1.0f + tmp[0];
			v = 1.0f - tmp[2];
			break;
		case 5:
			u = 1.0f - tmp[0];
			v = 1.0f - tmp[1];
			break;
		}
		return Vector3(u * 0.5f, v * 0.5f);
	}
};