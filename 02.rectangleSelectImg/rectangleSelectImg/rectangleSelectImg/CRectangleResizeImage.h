#pragma once
#include<stdio.h>
#include<stack>
#include<vector>
#include<algorithm>
#include "gdal_priv.h" 
#include "gdal_alg.h" 
#include<iostream>
#include <numeric>

using namespace std;

typedef struct Point
{
	int x;
	int y;
}Point;

typedef struct position
{
	Point center;
	int weight;
	int height;
}position;

class CRectangleResizeImage
{
public:
	CRectangleResizeImage();
	~CRectangleResizeImage();

public:	
	void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY, const char* pszFormat);//裁剪影像
	//1.统计影像中纵向连续值为0（1/2/3）的信息
	vector<vector<position>> staNumOfImgValue(string imgfile);
	//2.像元相邻值相同且连续进行合并
	vector<vector<position>> MergeRectangleImage(vector<vector<vector<int>>>& heights);
	//2.1连续且面积最大
	position CRectangleResizeImage::MaxAreaImage(vector<int>& vec, int Y);
	//3.求整景影像最大面积
	position CRectangleResizeImage::MaxAreaofAllImage(vector<position>& rowArea);
	//4.将合并的区域裁剪
	int ResizeImage(string imgfile, string oriImgfile, string outpath);

//public:
//	position centerPosition;
private:
	vector<vector<vector<int>>> statistics;
	vector<vector<position>> staRectangleArea;
	double m_GeoTransform[6];
	int m_imgWidth;
	int m_imgHeight;
	int m_imgBand;
};
