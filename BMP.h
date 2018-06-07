#pragma once

#include <iostream>
#include <fstream>
using namespace std;

void readParams(ifstream &os);
void readIMG(ifstream &os);
void writeParams(ofstream &os);
void writeIMG(ofstream &os);
void coder(const char* baseImgName, const char* newImgName, const char* letter);
void decoder(const char* baseImgName, const char* newImgName);

int color = 3;
struct BMP/*различные параметры файла формата bmp*/{
	unsigned int imgWidth = 500, imgHeight = 500;
	unsigned char signature[2] = { 'B', 'M' };
	unsigned int fileSize = 14 + 40 + imgWidth * imgHeight * color;
	unsigned int reserved = 0;
	unsigned int offset = 14 + 40;
	unsigned int headerSize = 40;
	unsigned int dimensions[2] = { imgWidth, imgHeight };
	unsigned short colorPlanes = 1;
	unsigned short bpp = 32;
	unsigned int compression = 0;
	unsigned int imgSize = imgWidth * imgHeight * color;
	unsigned int resolution[2] = { 2795, 2795 };
	unsigned int pltColors = 0;
	unsigned int impColors = 0;
	int lineImgSize;
};

unsigned char *IMG;
BMP settings;

