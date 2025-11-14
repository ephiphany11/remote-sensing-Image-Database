#pragma once
#include <iostream>
#include "gdal_priv.h" 
#include "gdal_alg.h" 

using namespace std;

class CImgScreen
{
public:
	CImgScreen();
	~CImgScreen();
	bool ImgScreen(string inPutFilePath);
};

