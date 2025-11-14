#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <set>
#include "gdal_priv.h" 
#include "gdal_alg.h" 
#include "gdalwarper.h"
#include "ogrsf_frmts.h"
#include "io.h"
#include <fstream>
#include <numeric> 
#include <algorithm>
#include "ogr_spatialref.h"

using namespace std;

typedef struct Point
{
	double x;
	double y;
}Point;

typedef struct Grid_Info
{
	Point Left_Top_Point;
	Point Right_Top_Point;
	Point Left_Bottom_Point;
	Point Right_Bottom_Point;
	int Value;

}Grid_Info;

typedef struct maxminXY
{
	double minX;
	double minY;
	double maxX;
	double maxY;	
}maxminXY;

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

	// 初始化索引向量
	vector<size_t> idx(v.size());
	//使用iota对向量赋0~？的连续值
	iota(idx.begin(), idx.end(), 0);

	// 通过比较v的值对索引idx进行排序
	sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
	return idx;
}


class CImgSelectBFS
{
public:
	CImgSelectBFS();
	~CImgSelectBFS();
	void firstImgByTime(int targetTime, vector<string> &imgsPath);//按感兴趣时间搜索第一景影像，将其作为起点影像集
	string biggestAreaImg(vector<string> &imgsPath, string &shpfile);//在搜索的同一感兴趣时刻影像集中，搜索覆盖目标区域面积最大的一景影像作为起点影像	

public:
	vector<string> interestingTimeSort(int targetTime,vector<string> &imgsPath);//基于感兴趣时间对数据进行排序
	
	vector<string> SelectImage(int targetTime, vector<string> &InputNamePath, vector<string> &imgsPath, string & shpfile, string & OutPath);
	vector<string> ImgSortByArea(vector<string> &imgsPath, string &shpfile);//对同一感兴趣时刻影像集中，按照搜索覆盖目标区域面积从大到小进行排序
	int ReadShpFile(string &shpfile, vector<Point> &point); //读取区域shp文件
	Grid_Info** CreatGrid(double &Xmin, double &Xmax, double &Ymin, double &Ymax, double &GridSpace_X, double &GridSpace_Y);//创建格网
	void DestoryGrid(Grid_Info** p);//清除格网
	bool PtInPolygon(Point p, vector<Point> &ptPolygon, int nCount);  //判断点在多边形内
	maxminXY readImgXY(string &imgPath);//读取影像的最大最小XY坐标
	int imageSize(string &imgPath);//读取裁剪后的影像的大小

	//2022-09-04新建
	vector<string> SelectImageByICRA(int targetTime, string &imgShpFilePath, vector<string> &imgsPath, string & shpfile, string & OutPath);
	int ReadPolyFile(string &polyfile, vector<Point> &point);
	//云量检测——对影像进行降采样，然后使用阈值分割，对影像进行快速云量检测
	int img_dcloud(string imgfile, string pszOutFile);
	int ResampleGDAL(string imgfile, string pszOutFile, float fResX, float fResY, GDALResampleAlg eResample);
	vector<string> backCountSort(vector<string>& imgsPath);

	GDALDataset *ThumbnailsDataSet;  //缩略图的数据集
	float iterate(GUInt16 *image_in, int rows, int cols);
	vector<string> interestingTimeSort(int targetTime, vector<string> &imgsPath, string & imgPath);//基于感兴趣时间对数据进行排序

};

