#pragma once

class Face {
public:
	int vidx1;
	int vidx2;
	int vidx3;

	Face(int vidx1, int vidx2, int vidx3)
	{
		this->vidx1 = vidx1;
		this->vidx2 = vidx2;
		this->vidx3 = vidx3;
	}
};