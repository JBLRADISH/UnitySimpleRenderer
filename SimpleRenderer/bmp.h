#pragma once

#include <Windows.h>
#include "texture.h"
#include <string>
#include <iostream>
using namespace std;

class Bmp
{
public:
	static Texture Load(const string& filename)
	{
		Texture res;
		FILE* fp;
		BITMAPFILEHEADER header;
		RGBQUAD palette[256];
		BITMAPINFOHEADER info;
		fp = fopen(filename.c_str(), "rb");
		if (fp != NULL)
		{
			//cout << "位图文件名:" << filename << endl;
			fread(&header, 1, sizeof(BITMAPFILEHEADER), fp);
			//showBmpHead(header);
			if (0x4d42 != header.bfType)
			{
				cout << filename << " is not a bmp file!" << endl;
				return res;
			}
			fread(&info, 1, sizeof(BITMAPINFOHEADER), fp);
			//showBmpInforHead(info);
			for (unsigned int nCounti = 0; nCounti < info.biClrUsed; nCounti++)
			{
				fread((char*)& palette[nCounti].rgbBlue, 1, sizeof(BYTE), fp);
				fread((char*)& palette[nCounti].rgbGreen, 1, sizeof(BYTE), fp);
				fread((char*)& palette[nCounti].rgbRed, 1, sizeof(BYTE), fp);
				cout << "strPla[nCounti].rgbBlue" << palette[nCounti].rgbBlue << endl;
				cout << "strPla[nCounti].rgbGreen" << palette[nCounti].rgbGreen << endl;
				cout << "strPla[nCounti].rgbRed" << palette[nCounti].rgbRed << endl;
			}
			res.width = info.biWidth;
			res.height = info.biHeight;
			res.SetMipMapLevel(log2f(fminf(res.width, res.height)));
			res.buffer[0] = (BYTE*)calloc(sizeof(BYTE), info.biSizeImage);
			fseek(fp, -(int)(info.biSizeImage), SEEK_END);
			fread(res.buffer[0], sizeof(BYTE), info.biSizeImage, fp);
			fclose(fp);
			//flip bmp
			int bytes_per_line = sizeof(BYTE) * info.biBitCount / 8 * info.biWidth;
			BYTE* tmp = (BYTE*)malloc(info.biSizeImage);
			memcpy(tmp, res.buffer[0], info.biSizeImage);
			for (int i = 0; i < info.biHeight; i++)
			{
				memcpy(&res.buffer[0][((info.biHeight - 1) - i) * bytes_per_line], &tmp[i * bytes_per_line], bytes_per_line);
			}
			free(tmp);
			res.GenerateMipMap();
			return res;
		}
		else
		{
			cout << filename << " open error!" << endl;
			return res;
		}
	}

private:
	static void showBmpHead(BITMAPFILEHEADER header)
	{
		cout << "位图文件头:" << endl;
		cout << "文件类型:" << header.bfType << endl;
		cout << "文件大小:" << header.bfSize << endl;
		cout << "保留字_1:" << header.bfReserved1 << endl;
		cout << "保留字_2:" << header.bfReserved2 << endl;
		cout << "实际位图数据的偏移字节数:" << header.bfOffBits << endl;
	}

	static void showBmpInforHead(BITMAPINFOHEADER info)
	{
		cout << "位图信息头:" << endl;
		cout << "结构体的长度:" << info.biSize << endl;
		cout << "位图宽:" << info.biWidth << endl;
		cout << "位图高:" << info.biHeight << endl;
		cout << "biPlanes平面数:" << info.biPlanes << endl;
		cout << "biBitCount采用颜色位数:" << info.biBitCount << endl;
		cout << "压缩方式:" << info.biCompression << endl;
		cout << "biSizeImage实际位图数据占用的字节数:" << info.biSizeImage << endl;
		cout << "X方向分辨率:" << info.biXPelsPerMeter << endl;
		cout << "Y方向分辨率:" << info.biYPelsPerMeter << endl;
		cout << "使用的颜色数:" << info.biClrUsed << endl;
		cout << "重要颜色数:" << info.biClrImportant << endl << endl;
	}
};