#include<iostream>
#include<stdio.h>
#include "CImgSelectBFS.h"
#include "CfileSelect.h"
#include <windows.h>

using namespace std;

int main()
{
	vector<string> inputFile;
	vector<string> imgs;
	vector<string> resultImgs;
	//string inputFilePath01 = "G:\\05.GF1.WFV.16m.Hubei\\03.GF1_WFV_16m_Hubei_8bit";//按与目标时间的差值，从小到大排序
	string inputFilePath02 = "H:\\Data.20230911\\02_CB04.10m.Hubei\\02.data";
	//string inputFilePath03 = "G:\\01_hubei251\\04\\201506";
	//string imgsPath01 = "G:\\05.GF1.WFV.16m.Hubei\\09.selectResultDLimgs";//待筛选的数据集
	string imgsPath02 = "H:\\Data.20230911\\02_CB04.10m.Hubei\\06.rectangleImg_Good";//待筛选的数据集
	//string imgsPath03 = "G:\\03_DL_results\\201506";//待筛选的数据集
	//string imgsPath04 = "G:\\03_DL_results\\201510";//待筛选的数据集
	//string imgsPath05 = "G:\\03_DL_results\\201511";//待筛选的数据集
	//string imgsPath06 = "G:\\03_DL_results\\201512";//待筛选的数据集
	string hubeiShpPath = "G:\\01.GF1-2015-huibei251\\00.hubei.shp\\hubei_poloy.shp";//湖北省的矢量图	
//	string imgShpFilePath = "J:\\11.imageSelectByICRA\\1.imageCut.251.poly\\";//影像的poly目录
	string OutPath = "H:\\Data.20230911\\02_CB04.10m.Hubei\\07.GF1CB04selectResult\\";//输出结果路径

	CfileSelect fileSelect;
	/*fileSelect.fileSearch(inputFilePath01, inputFile);*/
	fileSelect.fileSearch(inputFilePath02, inputFile);
	//fileSelect.fileSearch(inputFilePath03, inputFile);
	cout << inputFile.size() << endl;
	/*fileSelect.fileSearch(imgsPath01, imgs);*/
	fileSelect.fileSearch(imgsPath02, imgs);
	//fileSelect.fileSearch(imgsPath03, imgs);
	/*fileSelect.fileSearch(imgsPath04, imgs);
	fileSelect.fileSearch(imgsPath05, imgs);
	fileSelect.fileSearch(imgsPath06, imgs);*/
	cout << imgs.size() << endl;
	
	cout << "***开始筛选***"<< endl;
	CImgSelectBFS ImgSelectBFS;
	resultImgs=ImgSelectBFS.SelectImage(665, inputFile,imgs, hubeiShpPath, OutPath);

	
/*	//2022-09-04新增 ICRA方法
	fileSelect.fileSearch(imgsPath, imgs);
	resultImgs = ImgSelectBFS.SelectImageByICRA(365, imgShpFilePath, imgs, hubeiShpPath, OutPath);
*/
	
	/*************************1.2将结果文件输出**************************/

	string file = OutPath + "20211101_selectResults"+ ".txt";//20150901_selectResults
	FILE * fp = NULL;

	fopen_s(&fp, file.c_str(), "w");
	if (fp == NULL) {
		cout << "file open fail" << endl;
		return -1;
	}

	for (int i = 0; i != resultImgs.size(); i++) 
	{
		fprintf(fp, "%s\n", resultImgs[i].c_str());
	}

	fclose(fp);

	return 0;
}