#pragma once
#include<stdio.h>
#include<stack>
#include<vector>
#include<algorithm>
#include "gdal_priv.h" 
#include "gdal_alg.h" 
#include<iostream>

using namespace std;

typedef struct Point
{
	int x;
	int y;
}Point;

//typedef struct position 
//{
//	Point center;
//	int left;
//	int right;
//	int high;
//
//}position;


class CRectangleArea
{
public:
	CRectangleArea();
	CRectangleArea(string imgfile, string oriImgfile, string outpath);
	~CRectangleArea();

public:
	vector<position> RectangleArea(vector<vector<int>>& heights);//筛选最大的面积
	vector<vector<int>> statisticsNumOfImg(string imgfile);
	void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY, const char* pszFormat);//裁剪影像

public:
	position centerPosition;
private:
	vector<vector<int>> statistics;
	vector<position> staRectangleArea;
	double m_GeoTransform[6];
	int m_imgWidth;
	int m_imgHeight;
	int m_imgBand;
};