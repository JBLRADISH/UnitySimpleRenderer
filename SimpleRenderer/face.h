#pragma once

class Face {
public:
	int vidx1;
	int vidx2;
	int vidx3;
	int uvidx1;
	int uvidx2;
	int uvidx3;
	int state; // 0表示被背面剔除 1表示正常显示

	Face(int vidx1, int vidx2, int vidx3, int uvidx1, int uvidx2, int uvidx3)
	{
		this->vidx1 = vidx1;
		this->vidx2 = vidx2;
		this->vidx3 = vidx3;
		this->uvidx1 = uvidx1;
		this->uvidx2 = uvidx2;
		this->uvidx3 = uvidx3;
		state = 1;
	}
};