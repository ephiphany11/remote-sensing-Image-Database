#include "CImgScreen.h"



CImgScreen::CImgScreen()
{
}


CImgScreen::~CImgScreen()
{
}

bool CImgScreen::ImgScreen(string inPutFilePath)
{
	bool select = false;
	bool tmp01 = false;
	bool tmp02 = false;
	bool tmp03 = false; 
	bool tmp04 = false;
	bool tmp05 = false;
	bool tmp06 = false;
	bool tmp07 = false;
	bool tmp08 = false;
	bool tmp09 = false;
	bool tmp10 = false;

	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径														
	GDALDataset *poDataset;//GDAL数据集							
	poDataset = (GDALDataset*)GDALOpen(inPutFilePath.c_str(), GA_ReadOnly);

	//确认影像是否读入	
	if (poDataset == NULL)
	{
		cout << "影像打开失败!" << endl;
	}
	else {
		cout << "影像打开成功!" << endl;
	}

	//获取影像的长、宽、波段数及投影信息
	int m_imgBand = poDataset->GetRasterCount();//这里为1
	int m_imgWidth = poDataset->GetRasterXSize();
	int m_imgHeight = poDataset->GetRasterYSize();

	//图像坐标信息	
	double adfGeoTransform[6] = { 0 };
	poDataset->GetGeoTransform(adfGeoTransform);
	
	// 图像的左上角坐标、右下角坐标
	double leftUpX = adfGeoTransform[0];
	double leftUpY = adfGeoTransform[3];
	adfGeoTransform[0] = adfGeoTransform[0] + m_imgWidth*adfGeoTransform[1] + m_imgHeight*adfGeoTransform[2];
	adfGeoTransform[3] = adfGeoTransform[3] + m_imgWidth*adfGeoTransform[4] + m_imgHeight*adfGeoTransform[5];

	double rightDownX = adfGeoTransform[0];
	double rightDownY = adfGeoTransform[3];

	cout << leftUpX << " " << leftUpY << endl;
	cout << rightDownX << " " << rightDownY << endl;

	if ((leftUpX >108.36 && leftUpX <116.14) && (leftUpY >29.03 && leftUpY <33.12))
	{
		tmp01 = true;
	}

	if ((rightDownX >108.36 && rightDownX <116.14) && (rightDownY >29.03 && rightDownY <33.12))
	{
		tmp02 = true;
	}

	if ((rightDownX >108.36 && rightDownX <116.14) && (leftUpY >29.03 && leftUpY <33.12))
	{
		tmp03 = true;
	}

	if ((leftUpX >108.36 && leftUpX <116.14 )&& (rightDownY>29.03 && rightDownY <33.12))
	{
		tmp04 = true;
	}

	if ((leftUpX <=108.36 && leftUpY >= 33.12) && (rightDownX>= 108.36 && rightDownY <= 29.03))
	{
		tmp05 = true;
	}

	if ((leftUpX >= 108.36 && leftUpX <= 116.14 && leftUpY >= 33.12) && (rightDownX >= 108.36 &&rightDownX <= 116.14 && rightDownY <= 29.03))
	{
		tmp06 = true;
	}

	if ((leftUpX <= 116.14 && leftUpY >= 33.12) && (rightDownX >= 116.14 && rightDownY <= 29.03))
	{
		tmp07 = true;
	}

	if ((leftUpX <= 108.36 && leftUpY >= 33.12) && (rightDownX >= 116.14 && rightDownY <= 33.12))
	{
		tmp08 = true;
	}

	if ((leftUpX <= 108.36 && leftUpY <= 33.12 && leftUpY >= 29.03) && (rightDownX >= 116.14 && rightDownY <= 33.12 && rightDownY >= 29.03))
	{
		tmp09 = true;
	}

	if ((leftUpX <= 108.36 && leftUpY >= 29.03) && (rightDownX >= 116.14 && rightDownY <= 29.03))
	{
		tmp10 = true;
	}

	if (tmp01 || tmp02 || tmp03 || tmp04 ||tmp05 || tmp06 || tmp07 || tmp08 || tmp09 || tmp10)
	{
		select = true;
	}

	return select;
}
