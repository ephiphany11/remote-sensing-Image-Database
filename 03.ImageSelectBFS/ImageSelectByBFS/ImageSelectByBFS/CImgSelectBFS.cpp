#include "CImgSelectBFS.h"



CImgSelectBFS::CImgSelectBFS()
{
}


CImgSelectBFS::~CImgSelectBFS()
{
}

void CImgSelectBFS::firstImgByTime(int targetTime, vector<string> &imgsPath)
{
	int timeDistance = 2000;
	vector<int> timeDistance_box;
	vector<string> shortestTimeimgsPath;
	for (int i = 0; i < imgsPath.size(); i++)
	{
		int tmp_year = 0;
		int tmp_month = 0;
		int tmp_day = 0;
		int tmp_time = 0;
		
		string time = imgsPath[i].substr(imgsPath[i].rfind("_201"), imgsPath[i].find("_L1") - imgsPath[i].find("_201"));
		string year = time.substr(1, 4);
		string month = time.substr(5, 2);
		string day = time.substr(7, 2);

		if (year == "2016")
		{
			tmp_year = 365;
		}
		tmp_month = (atoi(month.c_str()) - 1) * 30;
		tmp_day = (atoi(day.c_str()));
		tmp_time = tmp_year + tmp_month + tmp_day;

		int tmp_timeDistance = abs(tmp_time - targetTime);
		timeDistance_box.push_back(tmp_timeDistance);

		if (timeDistance > tmp_timeDistance)
		{
			timeDistance = tmp_timeDistance;
		}
	}
	
	for (int i = 0; i < timeDistance_box.size(); i++)
	{
		if (timeDistance_box[i] == timeDistance)
		{
			shortestTimeimgsPath.push_back(imgsPath[i]);
		}
	}


	for (int i = 0; i < shortestTimeimgsPath.size(); i++)
	{
		cout << shortestTimeimgsPath[i]<<endl;
	}

}

string CImgSelectBFS::biggestAreaImg(vector<string> &imgsPath, string &shpfile)
{
	//获取区域
	vector<Point> AreaPoint;
	int ret = ReadShpFile(shpfile, AreaPoint);
	if (ret != 0)
	{
		cout<<"ReadShpFile false"<< endl;
	}
	//获取选定区域的最小外接矩形
	double Xmin, Xmax, Ymin, Ymax;
	Xmin = AreaPoint[0].x;
	Xmax = AreaPoint[0].x;
	Ymin = AreaPoint[0].y;
	Ymax = AreaPoint[0].y;
	for (int i = 1; i != AreaPoint.size(); i++)
	{
		if (Xmin > AreaPoint[i].x)
		{
			Xmin = AreaPoint[i].x;
		}
		if (Xmax < AreaPoint[i].x)
		{
			Xmax = AreaPoint[i].x;
		}
		if (Ymin > AreaPoint[i].y)
		{
			Ymin = AreaPoint[i].y;
		}
		if (Ymax < AreaPoint[i].y)
		{
			Ymax = AreaPoint[i].y;
		}
	}

	double GridSpace_X = 0.0;
	double GridSpace_Y = 0.0;
	Grid_Info** pGrid = NULL;

	//创建格网，读取湖北省区域，设置为200*200
	pGrid = CreatGrid(Xmin, Xmax, Ymin, Ymax, GridSpace_X, GridSpace_Y);
	if (pGrid == NULL)
	{
		cout << "CreatGrid fail"<< endl;
	}
	for (int i = 0; i != 500; i++)//判断格网是否在矢量范围内
	{
		for (int j = 0; j != 500; j++)
		{
			if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Left_Top_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Top_Point, AreaPoint, AreaPoint.size()) == true))
			{
				pGrid[i][j].Value = 1;			
			}
			else
			{
				pGrid[i][j].Value = 0;
			}
		}
	}
	vector<int> count;
	for (int i = 0; i < imgsPath.size(); i++) //比较影像覆盖范围的大小
	{
		maxminXY img_XY;
		vector<Point> image_poly;	
		img_XY = readImgXY(imgsPath[i]);
		double Image_Xmin, Image_Xmax, Image_Ymin, Image_Ymax;

		int area_count = 0;

		Point point01, point02, point03, point04;
		point01.x = img_XY.minX;
		point01.y = img_XY.maxY;
		image_poly.push_back(point01);

		point02.x = img_XY.maxX;
		point02.y = img_XY.maxY;
		image_poly.push_back(point02);

		point03.x = img_XY.maxX;
		point03.y = img_XY.minY;
		image_poly.push_back(point03);

		point04.x = img_XY.minX;
		point04.y = img_XY.minY;
		image_poly.push_back(point04);

		Image_Xmin = img_XY.minX;
		Image_Xmax = img_XY.maxX;
		Image_Ymin = img_XY.minY;
		Image_Ymax = img_XY.maxY;

		int ibegin = ((Image_Xmin - Xmin) / GridSpace_X) - 5;
		int iend = ((Image_Xmax - Xmin) / GridSpace_X) + 5;
		int jbegin = ((Image_Ymin - Ymin) / GridSpace_Y) - 5;
		int jend = ((Image_Ymax - Ymin) / GridSpace_Y) + 5;
		if (ibegin < 0)
		{
			ibegin = 0;
		}
		else if (ibegin > 500)
		{
			ibegin = 500;
		}
		if (iend < 0)
		{
			iend = 0;
		}
		else if (iend > 500)
		{
			iend = 500;
		}
		if (jbegin < 0)
		{
			jbegin = 0;
		}
		else if (jbegin > 500)
		{
			jbegin = 500;
		}
		if (jend < 0)
		{
			jend = 0;
		}
		else if (jend > 500)
		{
			jend = 500;
		}
		for (unsigned int i = ibegin; i != iend; i++)
		{
			for (unsigned int j = jbegin; j != jend; j++)
			{
				if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Left_Top_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Top_Point, image_poly, image_poly.size()) == true))
				{
					if (pGrid[i][j].Value == 1)
					{
						area_count++;
					}
				}
			}
		}
		count.push_back(area_count);
	}
	
	DestoryGrid(pGrid);

	int maxPosition = max_element(count.begin(), count.end()) - count.begin();

	return imgsPath[maxPosition];//返回覆盖面积最大的影像的路径
}


//读取矢量文件
int CImgSelectBFS::ReadShpFile(string &shpfile, vector<Point> &Points)
{
	OGRRegisterAll();//注册 GDAL/OGR 中所有的驱动格式 
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRDataSource *pODS = NULL;
	pODS = OGRSFDriverRegistrar::Open(shpfile.c_str(), FALSE, NULL);
	if (pODS == NULL)
	{
		return -1;
		cout << "shp fail" << endl;
	}
	OGRLayer *pLayer = NULL;
	pLayer = pODS->GetLayer(0);//读取该数据源中的第一个图层 
	OGRFeatureDefn *pFeatureDefn = NULL;
	pFeatureDefn = pLayer->GetLayerDefn();//获取该图层的定义 
	OGRFeature *pFeature = NULL;
	pLayer->ResetReading();//从头开始读取 
	pFeature = pLayer->GetFeature(0);
	OGRGeometry *p1 = pFeature->GetGeometryRef();
	OGRwkbGeometryType geotype;
	geotype = p1->getGeometryType();//获取该要素的几何形状类型 
	if (wkbPolygon == geotype)//如果该要素为多边形 
	{
		OGRPolygon *pPolygon;
		pPolygon = (OGRPolygon*)p1;
		OGRLinearRing *pLinearRing = NULL;
		pLinearRing = pPolygon->getExteriorRing();//获取该多边形的外环 

		OGRPoint point;
		Point iPoint;
		int nPointNum = 0;
		nPointNum = pLinearRing->getNumPoints();//获取该多边形中外环的点的个数 
		for (int i = 0; i<nPointNum; i++)
		{
			pLinearRing->getPoint(i, &point);//获取外环中某个特定的点 
			iPoint.x = point.getX();//该点的 x 坐标 
			iPoint.y = point.getY();
			Points.push_back(iPoint);
		}
	}
	return 0;
}

Grid_Info ** CImgSelectBFS::CreatGrid(double & Xmin, double & Xmax, double & Ymin, double & Ymax, double & GridSpace_X, double & GridSpace_Y)
{
	Grid_Info **pGrid = NULL;

	double Xspace = (Xmax - Xmin) / 500 * 1.0;
	double Yspace = (Ymax - Ymin) / 500 * 1.0;
	GridSpace_X = Xspace;
	GridSpace_Y = Yspace;

	pGrid = new Grid_Info*[500];
	for (int i = 0; i != 500; i++)
	{
		pGrid[i] = new Grid_Info[500];
	}
	for (int i = 0; i != 500; i++)
	{
		for (int j = 0; j != 500; j++)
		{
			//格网四个角点坐标赋值
			pGrid[i][j].Left_Bottom_Point.x = i*Xspace + Xmin;
			pGrid[i][j].Left_Bottom_Point.y = j*Yspace + Ymin;
			pGrid[i][j].Left_Top_Point.x = i*Xspace + Xmin;
			pGrid[i][j].Left_Top_Point.y = (j + 1)*Yspace + Ymin;
			pGrid[i][j].Right_Top_Point.x = (i + 1)*Xspace + Xmin;
			pGrid[i][j].Right_Top_Point.y = (j + 1)*Yspace + Ymin;
			pGrid[i][j].Right_Bottom_Point.x = (i + 1)*Xspace + Xmin;
			pGrid[i][j].Right_Bottom_Point.y = j*Yspace + Ymin;

			//格网值初始化
			pGrid[i][j].Value = 0;
		}
	}
	return pGrid;
}

void CImgSelectBFS::DestoryGrid(Grid_Info ** p)
{
	for (int i = 0; i != 500; i++)
	{
		delete[] p[i];
	}
	delete[] p;
}

bool CImgSelectBFS::PtInPolygon(Point p, vector<Point>& ptPolygon, int nCount)
{
	int nCross = 0;
	for (int i = 0; i < nCount; i++)
	{
		Point p1 = ptPolygon[i];
		Point p2 = ptPolygon[(i + 1) % nCount];
		if (p1.y == p2.y)
			continue;
		if (p.y < min(p1.y, p2.y))
			continue;
		if (p.y > max(p1.y, p2.y))
			continue;
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;

		if (x > p.x)
			nCross++;
	}
	return (nCross % 2 == 1);
}

maxminXY CImgSelectBFS::readImgXY(string & imgPath)
{
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径
	GDALDataset *m_pDataset = NULL;
	m_pDataset = (GDALDataset *)GDALOpen(imgPath.c_str(), GA_ReadOnly);
	if (m_pDataset == NULL) {
		cout << "image open fail" << endl;
	}
	GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	if (pDriver == NULL)
	{
		GDALClose((GDALDatasetH)m_pDataset);
	}
	
	GDALDataType dataType = m_pDataset->GetRasterBand(1)->GetRasterDataType();

	char *pszSrcWKT = NULL;
	pszSrcWKT = const_cast<char *>(m_pDataset->GetProjectionRef());

	int nNewWidth = m_pDataset->GetRasterXSize();
	int nNewHeight = m_pDataset->GetRasterYSize();
	int m_bandNum = m_pDataset->GetRasterCount();
	// 读取影像的投影信息
	const char* pszProjection = m_pDataset->GetProjectionRef();
	double m_GeoTransform[6];//投影转换参数
	m_pDataset->GetGeoTransform(m_GeoTransform);
	
	// 创建源和目标空间参考系统
	OGRSpatialReference oSourceSRS, oTargetSRS;
	// 假设源坐标系为EPSG:32649（WGS 84 / UTM zone 33N）
	oSourceSRS.importFromEPSG(32649);
	// 目标坐标系为WGS84
	oTargetSRS.importFromEPSG(4326);

	// 创建坐标转换
	OGRCoordinateTransformation* poCT;
	poCT = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
	double img_minX = m_GeoTransform[0];
	double img_maxY = m_GeoTransform[3];

	poCT->Transform(1, &img_minX, &img_maxY);
	// 释放坐标转换对象
	OCTDestroyCoordinateTransformation(poCT);

	double img_maxX = m_GeoTransform[0]+ nNewWidth * m_GeoTransform[1];
	double img_minY = m_GeoTransform[3]+ nNewHeight * m_GeoTransform[5];

	maxminXY imgXY;
	imgXY.minX = img_minX;
	imgXY.maxX = img_maxX;
	imgXY.minY = img_minY;
	imgXY.maxY = img_maxY;

	return imgXY;
}

int CImgSelectBFS::imageSize(string & imgPath)
{
	int size = 0;
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径
	GDALDataset *m_pDataset = NULL;
	m_pDataset = (GDALDataset *)GDALOpen(imgPath.c_str(), GA_ReadOnly);
	if (m_pDataset == NULL) {
		cout << "image open fail" << endl;
	}

	//获取影像的长、宽、波段数及投影信息
	int nBandCount = m_pDataset->GetRasterCount();
	int nImgSizeX = m_pDataset->GetRasterXSize();
	int nImgSizeY = m_pDataset->GetRasterYSize();

	GDALRasterBand *poBand = m_pDataset->GetRasterBand(1); //输入文件，读取第一波段

	//创建动态内存，存储第一波段影像													   
	unsigned char* pBuf = new unsigned char[nImgSizeX * nImgSizeY];
	memset(pBuf, 0, nImgSizeX * nImgSizeY * sizeof(unsigned char));
	poBand->RasterIO(GF_Read, 0, 0, nImgSizeX, nImgSizeY, pBuf, nImgSizeX, nImgSizeY, GDT_Byte, 0, 0);

	for (int i = 0; i < nImgSizeY; i++)
	{
		for (int j = 0; j < nImgSizeX; j++)
		{
			unsigned char* value = pBuf + i*nImgSizeX + j;
			if ((*value) != 0)
			{
				size++;
			}
		}
	}

	delete[] pBuf;
	pBuf = NULL;

	GDALClose(m_pDataset);
	GDALDestroyDriverManager();
	return size;
}


vector<string> CImgSelectBFS::SelectImageByICRA(int targetTime, string& imgShpFilePath, vector<string>& imgsPath, string & shpfile, string & OutPath)
{
	bool tmp = false;
	vector<string> imgsByTimeSort;
	vector<string> resultImgs;

	string outTmpPath = OutPath + "tmp\\";
	imgsByTimeSort = interestingTimeSort(targetTime, imgsPath, outTmpPath);
	int timeCount = imgsByTimeSort.size();
	
	vector<string>  imgsByTime;//2022-09-02使用						   

	for (int j = 0; j < imgsByTimeSort.size(); j++)
	{
		for (int i = 0; i < imgsPath.size(); i++)
		{
			string imgName = imgsPath[i].substr(imgsPath[i].rfind("E1"), imgsPath[i].find(".tif") - imgsPath[i].find("E1"));

			if (imgName == imgsByTimeSort[j])
			{
				imgsByTime.push_back(imgsPath[i]);
			}
			else { continue; }
		}
	}

	cout << "***全部数据已读取***" << endl;

	//创建目标-研究区域格网
	vector<Point> AreaPoint;
	int ret = ReadShpFile(shpfile, AreaPoint);
	if (ret != 0)
	{
		cout << "ReadShpFile false" << endl;
	}
	//获取选定区域的最小外接矩形
	double Xmin, Xmax, Ymin, Ymax;
	Xmin = AreaPoint[0].x;
	Xmax = AreaPoint[0].x;
	Ymin = AreaPoint[0].y;
	Ymax = AreaPoint[0].y;
	for (int i = 1; i != AreaPoint.size(); i++)
	{
		if (Xmin > AreaPoint[i].x)
		{
			Xmin = AreaPoint[i].x;
		}
		if (Xmax < AreaPoint[i].x)
		{
			Xmax = AreaPoint[i].x;
		}
		if (Ymin > AreaPoint[i].y)
		{
			Ymin = AreaPoint[i].y;
		}
		if (Ymax < AreaPoint[i].y)
		{
			Ymax = AreaPoint[i].y;
		}
	}

	double GridSpace_X = 0.0;
	double GridSpace_Y = 0.0;
	Grid_Info** pGrid = NULL;

	//创建格网，读取湖北省区域，设置为925*587
	int area_count = 0;
	pGrid = CreatGrid(Xmin, Xmax, Ymin, Ymax, GridSpace_X, GridSpace_Y);
	if (pGrid == NULL)
	{
		cout << "CreatGrid fail" << endl;
	}
	for (int i = 0; i != 500; i++)//判断格网是否在矢量范围内
	{
		for (int j = 0; j != 500; j++)
		{
			if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Left_Top_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Top_Point, AreaPoint, AreaPoint.size()) == true))
			{
				pGrid[i][j].Value = 1;
				area_count++;
			}
			else
			{
				pGrid[i][j].Value = 0;
			}
		}
	}

	cout << "***湖北省区域格网已创建成功***" << endl;


	//基于感兴趣时刻的筛选--209255872使用
	for (int i = 0; i <imgsByTime.size(); i++)
	{

		cout << "筛选第 " << i << " 景数据！" << endl;
		vector<Point> image_poly;
		string imgName = imgsByTime[i].substr(imgsByTime[i].rfind("hu"), imgsByTime[i].find(".tif") - imgsByTime[i].find("hu"));
		string tmp_name = imgShpFilePath + imgName + ".poly";

		ReadPolyFile(tmp_name, image_poly);

		int areaCount = 0;

		double Image_Xmin, Image_Xmax, Image_Ymin, Image_Ymax;

		Image_Xmin = image_poly[0].x;
		Image_Xmax = image_poly[0].x;
		Image_Ymin = image_poly[0].y;
		Image_Ymax = image_poly[0].y;

		for (int i = 1; i != image_poly.size(); i++)
		{
			if (Image_Xmin > image_poly[i].x)
			{
				Image_Xmin = image_poly[i].x;
			}
			if (Image_Xmax < image_poly[i].x)
			{
				Image_Xmax = image_poly[i].x;
			}
			if (Image_Ymin > image_poly[i].y)
			{
				Image_Ymin = image_poly[i].y;
			}
			if (Image_Ymax < image_poly[i].y)
			{
				Image_Ymax = image_poly[i].y;
			}
		}

		int ibegin = ((Image_Xmin - Xmin) / GridSpace_X) - 5;
		int iend = ((Image_Xmax - Xmin) / GridSpace_X) + 5;
		int jbegin = ((Image_Ymin - Ymin) / GridSpace_Y) - 5;
		int jend = ((Image_Ymax - Ymin) / GridSpace_Y) + 5;
		if (ibegin < 0)
		{
			ibegin = 0;
		}
		else if (ibegin > 500)
		{
			ibegin = 500;
		}
		if (iend < 0)
		{
			iend = 0;
		}
		else if (iend > 500)
		{
			iend = 500;
		}
		if (jbegin < 0)
		{
			jbegin = 0;
		}
		else if (jbegin > 500)
		{
			jbegin = 500;
		}
		if (jend < 0)
		{
			jend = 0;
		}
		else if (jend > 500)
		{
			jend = 500;
		}
		for (unsigned int i = ibegin; i != iend; i++)
		{
			for (unsigned int j = jbegin; j != jend; j++)
			{
				if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Left_Top_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Top_Point, image_poly, image_poly.size()) == true))
				{
					if (pGrid[i][j].Value == 1)
					{
						pGrid[i][j].Value = 2;
						areaCount++;
						area_count--;
					}
				}
			}
		}

		if (areaCount > 0)
		{
			resultImgs.push_back(imgsByTime[i]);
		}

		if (area_count <= 0)
		{
			break;
			tmp = true;
			cout << "完成全覆盖！！！" << endl;
		}

		cout << area_count << endl;

		if (tmp)
		{
			break;
		}

	}

	DestoryGrid(pGrid);
	return resultImgs;
}



vector<string> CImgSelectBFS::interestingTimeSort(int targetTime,vector<string>& imgsPath)
{
	int timeDistance = 2000000;
	vector<int> timeDistance_box;
	vector<size_t> idx;
	vector<string> imgsByTimeSort;

	for (int i = 0; i < imgsPath.size(); i++)
	{		
		//int size = imageSize(imgsPath[i]);//输入影像的面积
		//float size_T = float(size) / 200000000;
		int tmp_year = 0;
		int tmp_month = 0;
		int tmp_day = 0;
		int tmp_time = 0;

		string time = "";
		string tmp = imgsPath[i].substr(imgsPath[i].rfind("\\")+1, 2);
		if (tmp == "GF")
		{
			time = imgsPath[i].substr(imgsPath[i].rfind("_202"), imgsPath[i].find("_L1A") - imgsPath[i].find("_202"));
		}
		else if (tmp == "CB") 
		{
			time = imgsPath[i].substr(imgsPath[i].rfind("-202"), imgsPath[i].find("-L20") - imgsPath[i].find("-202"));
		}		
		string year = time.substr(1, 4);
		string month = time.substr(5, 2);
		string day = time.substr(7, 2);

		int year_int = atoi(year.c_str());
		int month_int = atoi(month.c_str());
		/*if (year == "9256")
		{
			tmp_year = 365;
		}*/

		tmp_year = (year_int - 2020) * 365;
		tmp_month = (month_int - 1) * 30;
		tmp_day = (atoi(day.c_str()));
		tmp_time = tmp_year + tmp_month + tmp_day;

		int tmp_timeDistance = abs(tmp_time - targetTime);
		/*int tmp_Distance = tmp_timeDistance / size_T;*/
		int tmp_Distance = tmp_timeDistance;
		timeDistance_box.push_back(tmp_Distance);
		//cout << i << "--" << imgsPath.size();
		//cout << "  size:" << size_T << "  timeD:" << tmp_timeDistance << "  tmp_Distance:" << tmp_Distance<<endl;
	}

	idx= sort_indexes(timeDistance_box); //对vector进行排序，并返回索引值

	for (int i = 0; i < timeDistance_box.size();i++)//name=E587.9_N33.5_92552001
	{
		//string name= imgsPath[idx[i]].substr(imgsPath[idx[i]].rfind("E1"), imgsPath[idx[i]].find("_L1") - imgsPath[idx[i]].find("E1"));
		string tmp = imgsPath[idx[i]].substr(imgsPath[idx[i]].rfind("\\")+1, 2);
		string name = "";
		if (tmp == "GF")
		{
			name = imgsPath[idx[i]].substr(imgsPath[idx[i]].rfind("GF1"), imgsPath[idx[i]].rfind(".tiff") - imgsPath[idx[i]].rfind("GF1"));
		}
		else if (tmp == "CB")
		{
			name = imgsPath[idx[i]].substr(imgsPath[idx[i]].rfind("CB04"), imgsPath[idx[i]].rfind(".TIF") - imgsPath[idx[i]].rfind("CB04"));
		}

		imgsByTimeSort.push_back(name);
	}

	return imgsByTimeSort;
}

vector<string> CImgSelectBFS::backCountSort(vector<string>& imgsPath)
{
	vector<float> backCount_box;
	vector<size_t> idx;
	vector<string> imgsBybackCountSort;

	for (int i = 0; i < imgsPath.size(); i++)
	{
		//1.背景
		int pos_back = imgsPath[i].length() - 9;
		string imgbackCount = imgsPath[i].substr(pos_back, 4);
		float tmp_backCount = stof(imgbackCount);
		//2.云
		int pos_cloud = imgsPath[i].length() - 18;
		string imgcloudCount = imgsPath[i].substr(pos_cloud, 4);
		float tmp_cloudCount = stof(imgcloudCount);
		float count = 0.15 * tmp_cloudCount + 0.85 * tmp_backCount;

		backCount_box.push_back(count);
	}
	idx = sort_indexes(backCount_box); //对vector进行排序，并返回索引值

	for (int i = 0; i < backCount_box.size(); i++)
	{
		string name = imgsPath[idx[i]];
		imgsBybackCountSort.push_back(name);
	}

	return imgsBybackCountSort;
}

vector<string> CImgSelectBFS::SelectImage(int targetTime, vector<string> &InputNamePath, vector<string> &imgsPath, string &shpfile, string &OutPath)
{
	clock_t start;
	start = clock();
	cout << "开始时间：" << start << endl;

	bool tmp = false;
	bool tpp = false;
	vector<string> imgsByTimeSort;
	vector<string> resultImgs;
	
	imgsByTimeSort = interestingTimeSort(targetTime, InputNamePath);
	int timeCount = imgsByTimeSort.size();
	vector<vector<vector<string>>> imgsByTime_Quality;
	//vector<vector<string> > imgsByTime(timeCount);
	vector<vector<string> > imgsByArea;

	//20240309修改
	//1.按照质量第一次排序
	//①-①质量为优--1
	vector<string> imgsByQua_1_1;
	vector<string> imgsByQua_2_4;
	vector<string> imgsByQua_3_2;
	vector<string> imgsByQua_4_3;
	vector<string> imgsByQua_4_3_new;
	for (int i = 0; i < imgsPath.size(); i++)
	{
		/*int pos = imgsPath[i].length() - 27;
		string imgQuality = imgsPath[i].substr(pos, 1);*/

		string imgQuality = imgsPath[i].substr(imgsPath[i].rfind("goo"), imgsPath[i].rfind("_") - imgsPath[i].rfind("goo"));;
		
		if (imgQuality == "good1")
		{
			imgsByQua_1_1.push_back(imgsPath[i]);
		}
		else if (imgQuality == "4")//①-②质量为薄云--4
		{
			imgsByQua_2_4.push_back(imgsPath[i]);
		}
		else if(imgQuality == "2")//①-③质量为厚云--2
		{
			imgsByQua_3_2.push_back(imgsPath[i]);
		}
		else if (imgQuality == "3")//①-③质量为背景--3
		{
			imgsByQua_4_3.push_back(imgsPath[i]);
		}
	}

	////把质量为背景的影像中 cloud=0,back=0的数据添加到（①-②质量为薄云）中
	//imgsByQua_4_3_new = imgsByQua_4_3;
	//for (int i = 0; i < imgsByQua_4_3.size(); i++)
	//{
	//	//1.背景
	//	int pos_back = imgsByQua_4_3[i].length() - 9;
	//	string imgbackCount = imgsByQua_4_3[i].substr(pos_back, 4);
	//	float tmp_backCount = stof(imgbackCount);
	//	//2.云
	//	int pos_cloud = imgsByQua_4_3[i].length() - 18;
	//	string imgcloudCount = imgsByQua_4_3[i].substr(pos_cloud, 4);
	//	float tmp_cloudCount = stof(imgcloudCount);
	//	
	//	if ((tmp_backCount == 0.00) && (tmp_cloudCount < 10.0))
	//	{
	//		imgsByQua_2_4.push_back(imgsByQua_4_3[i]);
	//		imgsByQua_4_3_new.erase(remove(imgsByQua_4_3_new.begin(), imgsByQua_4_3_new.end(), imgsByQua_4_3[i]), imgsByQua_4_3_new.end());
	//	}
	//}

	//vector<string> imgbackCountSort;
	//imgbackCountSort = backCountSort(imgsByQua_4_3_new);

	//20240309修改
	//2.质量后按照时间排序
	vector<vector<string> > imgsByTime_Quality_1_1(timeCount);
	for (int i = 0; i < imgsByQua_1_1.size(); i++)
	{
		/*string imgName_tmp1 = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("GF6"), imgsByQua_1_1[i].rfind("_") - imgsByQua_1_1[i].rfind("GF6"));
		string imgName_tmp2 = imgName_tmp1.substr(imgName_tmp1.rfind("GF6"), imgName_tmp1.rfind("_") - imgName_tmp1.rfind("GF6"));
		string imgName_tmp3 = imgName_tmp2.substr(imgName_tmp2.rfind("GF6"), imgName_tmp2.rfind("_") - imgName_tmp2.rfind("GF6"));
		string imgName = imgName_tmp3.substr(imgName_tmp3.rfind("GF6"), imgName_tmp3.rfind("_") - imgName_tmp3.rfind("GF6"));*/

		string imgName = "";
		string tmp = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("\\") + 1, 2);
		if (tmp == "GF")
		{
			imgName = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("GF1"), imgsByQua_1_1[i].rfind("_good") - imgsByQua_1_1[i].rfind("GF1"));
		}
		else if (tmp == "CB")
		{
			imgName = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("CB04"), imgsByQua_1_1[i].rfind("_good") - imgsByQua_1_1[i].rfind("CB04"));
		}
			
		for (int j = 0; j < imgsByTimeSort.size(); j++)
		{
			if (imgName == imgsByTimeSort[j])
			{
				imgsByTime_Quality_1_1[j].push_back(imgsByQua_1_1[i]);
			}
		}
	}
	imgsByTime_Quality.push_back(imgsByTime_Quality_1_1);

	vector<vector<string> > imgsByTime_Quality_2_4(timeCount);
	for (int i = 0; i < imgsByQua_2_4.size(); i++)
	{
		string imgName_tmp1 = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("GF6"), imgsByQua_1_1[i].rfind("_") - imgsByQua_1_1[i].rfind("GF6"));
		string imgName_tmp2 = imgName_tmp1.substr(imgName_tmp1.rfind("GF6"), imgName_tmp1.rfind("_") - imgName_tmp1.rfind("GF6"));
		string imgName_tmp3 = imgName_tmp2.substr(imgName_tmp2.rfind("GF6"), imgName_tmp2.rfind("_") - imgName_tmp2.rfind("GF6"));
		string imgName = imgName_tmp3.substr(imgName_tmp3.rfind("GF6"), imgName_tmp3.rfind("_") - imgName_tmp3.rfind("GF6"));
		for (int j = 0; j < imgsByTimeSort.size(); j++)
		{
			if (imgName == imgsByTimeSort[j])
			{
				imgsByTime_Quality_2_4[j].push_back(imgsByQua_2_4[i]);
			}
		}
	}
	imgsByTime_Quality.push_back(imgsByTime_Quality_2_4);

	vector<vector<string> > imgsByTime_Quality_3_2(timeCount);
	for (int i = 0; i < imgsByQua_3_2.size(); i++)
	{
		string imgName_tmp1 = imgsByQua_1_1[i].substr(imgsByQua_1_1[i].rfind("GF6"), imgsByQua_1_1[i].rfind("_") - imgsByQua_1_1[i].rfind("GF6"));
		string imgName_tmp2 = imgName_tmp1.substr(imgName_tmp1.rfind("GF6"), imgName_tmp1.rfind("_") - imgName_tmp1.rfind("GF6"));
		string imgName_tmp3 = imgName_tmp2.substr(imgName_tmp2.rfind("GF6"), imgName_tmp2.rfind("_") - imgName_tmp2.rfind("GF6"));
		string imgName = imgName_tmp3.substr(imgName_tmp3.rfind("GF6"), imgName_tmp3.rfind("_") - imgName_tmp3.rfind("GF6"));
		for (int j = 0; j < imgsByTimeSort.size(); j++)
		{
			if (imgName == imgsByTimeSort[j])
			{
				imgsByTime_Quality_3_2[j].push_back(imgsByQua_3_2[i]);
			}
		}
	}
	imgsByTime_Quality.push_back(imgsByTime_Quality_3_2);

	/*vector<vector<string> > imgsByback_Quality_4_3;
	imgsByback_Quality_4_3.push_back(imgbackCountSort);
	imgsByTime_Quality.push_back(imgsByback_Quality_4_3);*/

	////影像按照距感兴趣时间的远近，由近到远储存在二维vector---imgsByTime中
	////2022-09-02注释
	//for (int i = 0; i < imgsPath.size(); i++)
	//{
	//	//string imgName = imgsPath[i].substr(imgsPath[i].rfind("E1"), imgsPath[i].find("_L1") - imgsPath[i].find("E1"));
	//	string imgName_tmp = imgsPath[i].substr(imgsPath[i].rfind("GF6"), imgsPath[i].rfind("_") - imgsPath[i].rfind("GF6"));
	//	string imgName = imgName_tmp.substr(imgName_tmp.rfind("GF6"), imgName_tmp.rfind("_") - imgName_tmp.rfind("GF6"));
	//	cout << imgName << endl;
	//	for (int j = 0; j < imgsByTimeSort.size(); j++)
	//	{
	//		if (imgName == imgsByTimeSort[j])
	//		{
	//			imgsByTime[j].push_back(imgsPath[i]);
	//		}
	//	}		
	//}

	cout << "***全部数据已读取***" << endl;

	//创建研究区域格网
	vector<Point> AreaPoint;
	int ret = ReadShpFile(shpfile, AreaPoint);
	if (ret != 0)
	{
		cout << "ReadShpFile false" << endl;
	}
	//获取选定区域的最小外接矩形
	double Xmin, Xmax, Ymin, Ymax;
	Xmin = AreaPoint[0].x;
	Xmax = AreaPoint[0].x;
	Ymin = AreaPoint[0].y;
	Ymax = AreaPoint[0].y;
	for (int i = 1; i != AreaPoint.size(); i++)
	{
		if (Xmin > AreaPoint[i].x)
		{
			Xmin = AreaPoint[i].x;
		}
		if (Xmax < AreaPoint[i].x)
		{
			Xmax = AreaPoint[i].x;
		}
		if (Ymin > AreaPoint[i].y)
		{
			Ymin = AreaPoint[i].y;
		}
		if (Ymax < AreaPoint[i].y)
		{
			Ymax = AreaPoint[i].y;
		}
	}

	double GridSpace_X = 0.0;
	double GridSpace_Y = 0.0;
	Grid_Info** pGrid = NULL;

	//创建格网，读取湖北省区域，设置为500*500
	int area_count = 0;
	pGrid = CreatGrid(Xmin, Xmax, Ymin, Ymax, GridSpace_X, GridSpace_Y);
	if (pGrid == NULL)
	{
		cout << "CreatGrid fail" << endl;
	}
	for (int i = 0; i != 500; i++)//判断格网是否在矢量范围内
	{
		for (int j = 0; j != 500; j++)
		{
			if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Left_Top_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
				(PtInPolygon(pGrid[i][j].Right_Top_Point, AreaPoint, AreaPoint.size()) == true))
			{
				pGrid[i][j].Value = 1;
				area_count++;
			}
			else
			{
				pGrid[i][j].Value = 0;
			}
		}
	}

	cout << "***湖北省区域格网已创建成功***" << endl;
//	cout << "***网格大小--50pixels--925*587=542975***" << endl;
	//基于感兴趣时刻的筛选
	for(int qua =0; qua<imgsByTime_Quality.size(); qua++)
	{
		
		vector<vector<string> > imgsByTime(timeCount);
		imgsByTime = imgsByTime_Quality[qua];
		cout << "筛选质量为 " << qua << " 的数据！" << endl;
		for (int i = 0; i < imgsByTime.size(); i++)
		{
			int num = imgsByTime.size();
			if (num == 0)
			{
				continue;
			}
			//对imgsByTime中一个时间段内的影像，按照覆盖目标区域的大小进行排序，覆盖面积最大的在第一个
			vector<string> imgsByAreaSort;
			if (qua == 3)
			{
				imgsByAreaSort = imgsByTime[0];
			}
			else {
				imgsByAreaSort = ImgSortByArea(imgsByTime[i], shpfile);
			}
		
			cout << "筛选距离目标时刻第 " << i << " 近的时刻内数据！" << endl;
			cout << "----共包含有 " << imgsByAreaSort.size() << " 景数据！" << endl;

			for (int num = 0; num < imgsByAreaSort.size(); num++)
			{
				cout << "正在处理" << imgsByAreaSort[num] << endl;
				int areaCount = 0;
				maxminXY img_XY;
				vector<Point> image_poly;
				img_XY = readImgXY(imgsByAreaSort[num]);
				double Image_Xmin, Image_Xmax, Image_Ymin, Image_Ymax;

				Point point01, point02, point03, point04;
				point01.x = img_XY.minX;
				point01.y = img_XY.maxY;
				image_poly.push_back(point01);

				point02.x = img_XY.maxX;
				point02.y = img_XY.maxY;
				image_poly.push_back(point02);

				point03.x = img_XY.maxX;
				point03.y = img_XY.minY;
				image_poly.push_back(point03);

				point04.x = img_XY.minX;
				point04.y = img_XY.minY;
				image_poly.push_back(point04);

				Image_Xmin = img_XY.minX;
				Image_Xmax = img_XY.maxX;
				Image_Ymin = img_XY.minY;
				Image_Ymax = img_XY.maxY;

				int ibegin = ((Image_Xmin - Xmin) / GridSpace_X) - 5;
				int iend = ((Image_Xmax - Xmin) / GridSpace_X) + 5;
				int jbegin = ((Image_Ymin - Ymin) / GridSpace_Y) - 5;
				int jend = ((Image_Ymax - Ymin) / GridSpace_Y) + 5;
				if (ibegin < 0)
				{
					ibegin = 0;
				}
				else if (ibegin > 500)
				{
					ibegin = 500;
				}
				if (iend < 0)
				{
					iend = 0;
				}
				else if (iend > 500)
				{
					iend = 500;
				}
				if (jbegin < 0)
				{
					jbegin = 0;
				}
				else if (jbegin > 500)
				{
					jbegin = 500;
				}
				if (jend < 0)
				{
					jend = 0;
				}
				else if (jend > 500)
				{
					jend = 500;
				}
				for (unsigned int i = ibegin; i != iend; i++)
				{
					for (unsigned int j = jbegin; j != jend; j++)
					{
						if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, image_poly, image_poly.size()) == true) &&
							(PtInPolygon(pGrid[i][j].Left_Top_Point, image_poly, image_poly.size()) == true) &&
							(PtInPolygon(pGrid[i][j].Right_Bottom_Point, image_poly, image_poly.size()) == true) &&
							(PtInPolygon(pGrid[i][j].Right_Top_Point, image_poly, image_poly.size()) == true))
						{
							if (pGrid[i][j].Value == 1)
							{
								pGrid[i][j].Value = 2;
								areaCount++;
								area_count--;
							}
						}
					}
				}

				if (areaCount > 200)
				{
					resultImgs.push_back(imgsByAreaSort[num]);
				}

				if (area_count <= 200)
				{					
					tmp = true;
					tpp = true;
					cout << "完成全覆盖！！！" << endl;
					break;
				}
			}

			cout << area_count << endl;

			if (tmp)
			{
				break;
			}

			//计时
			clock_t t_time;
			t_time = clock();
			double endtime = (double)(t_time - start) / CLOCKS_PER_SEC;
			double endtime_min = (double)(endtime) / 60;
			double endtime_hour = (double)(endtime_min) / 60;
			cout << "完成时间：" << t_time << "， 共耗时：" << endtime << " 秒" << "， " << endtime_min << " 分钟" << "， " << endtime_hour << " 小时" << endl;
		}

		if (tpp)
		{
			break;
		}
	}
   
	/*
	//基于感兴趣时刻的筛选--209255872使用
	for (int i = 0; i <imgsByTime.size(); i++)
	{
	
		cout << "筛选第 " << i << " 景数据！" << endl;

		int areaCount = 0;
		maxminXY img_XY;
		vector<Point> image_poly;
		img_XY = readImgXY(imgsByTime[i]);
		double Image_Xmin, Image_Xmax, Image_Ymin, Image_Ymax;

		Point point01, point02, point03, point04;
		point01.x = img_XY.minX;
		point01.y = img_XY.maxY;
		image_poly.push_back(point01);

		point02.x = img_XY.maxX;
		point02.y = img_XY.maxY;
		image_poly.push_back(point02);

		point03.x = img_XY.maxX;
		point03.y = img_XY.minY;
		image_poly.push_back(point03);

		point04.x = img_XY.minX;
		point04.y = img_XY.minY;
		image_poly.push_back(point04);

		Image_Xmin = img_XY.minX;
		Image_Xmax = img_XY.maxX;
		Image_Ymin = img_XY.minY;
		Image_Ymax = img_XY.maxY;

		int ibegin = ((Image_Xmin - Xmin) / GridSpace_X) - 5;
		int iend = ((Image_Xmax - Xmin) / GridSpace_X) + 5;
		int jbegin = ((Image_Ymin - Ymin) / GridSpace_Y) - 5;
		int jend = ((Image_Ymax - Ymin) / GridSpace_Y) + 5;
		if (ibegin < 0)
		{
			ibegin = 0;
		}
		else if (ibegin > 925)
		{
			ibegin = 925;
		}
		if (iend < 0)
		{
			iend = 0;
		}
		else if (iend > 925)
		{
			iend = 925;
		}
		if (jbegin < 0)
		{
			jbegin = 0;
		}
		else if (jbegin > 587)
		{
			jbegin = 587;
		}
		if (jend < 0)
		{
			jend = 0;
		}
		else if (jend > 587)
		{
			jend = 587;
		}
		for (unsigned int i = ibegin; i != iend; i++)
		{
			for (unsigned int j = jbegin; j != jend; j++)
			{
				if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Left_Top_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Bottom_Point, image_poly, image_poly.size()) == true) &&
					(PtInPolygon(pGrid[i][j].Right_Top_Point, image_poly, image_poly.size()) == true))
				{
					if (pGrid[i][j].Value == 1)
					{
						pGrid[i][j].Value = 2;
						areaCount++;
						area_count--;
					}
				}
			}
		}

		if (areaCount > 0)
		{
			resultImgs.push_back(imgsByTime[i]);
		}

		if (area_count <= 0)
		{
			break;
			tmp = true;
			cout << "完成全覆盖！！！" << endl;
		}

		cout << area_count << endl;

		if (tmp)
		{
			break;
		}

	}
	*/	

	DestoryGrid(pGrid);
	return resultImgs;
}

//vector<string> CImgSelectBFS::ImgSortByArea(vector<string>& imgsPath, string & shpfile)
//{
//	//获取区域
//	vector<string> imgSortByArea;
//	vector<Point> AreaPoint;
//	vector<size_t> idx;
//	int ret = ReadShpFile(shpfile, AreaPoint);
//	if (ret != 0)
//	{
//		cout << "ReadShpFile false" << endl;
//	}
//	//获取选定区域的最小外接矩形
//	double Xmin, Xmax, Ymin, Ymax;
//	Xmin = AreaPoint[0].x;
//	Xmax = AreaPoint[0].x;
//	Ymin = AreaPoint[0].y;
//	Ymax = AreaPoint[0].y;
//	for (int i = 1; i != AreaPoint.size(); i++)
//	{
//		if (Xmin > AreaPoint[i].x)
//		{
//			Xmin = AreaPoint[i].x;
//		}
//		if (Xmax < AreaPoint[i].x)
//		{
//			Xmax = AreaPoint[i].x;
//		}
//		if (Ymin > AreaPoint[i].y)
//		{
//			Ymin = AreaPoint[i].y;
//		}
//		if (Ymax < AreaPoint[i].y)
//		{
//			Ymax = AreaPoint[i].y;
//		}
//	}
//
//	double GridSpace_X = 0.0;
//	double GridSpace_Y = 0.0;
//	Grid_Info** pGrid = NULL;
//
//	//创建格网，读取湖北省区域，设置为500*500
//	pGrid = CreatGrid(Xmin, Xmax, Ymin, Ymax, GridSpace_X, GridSpace_Y);
//	if (pGrid == NULL)
//	{
//		cout << "CreatGrid fail" << endl;
//	}
//	for (int i = 0; i != 500; i++)//判断格网是否在矢量范围内
//	{
//		for (int j = 0; j != 500; j++)
//		{
//			if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
//				(PtInPolygon(pGrid[i][j].Left_Top_Point, AreaPoint, AreaPoint.size()) == true) &&
//				(PtInPolygon(pGrid[i][j].Right_Bottom_Point, AreaPoint, AreaPoint.size()) == true) &&
//				(PtInPolygon(pGrid[i][j].Right_Top_Point, AreaPoint, AreaPoint.size()) == true))
//			{
//				pGrid[i][j].Value = 1;
//			}
//			else
//			{
//				pGrid[i][j].Value = 0;
//			}
//		}
//	}
//	vector<int> count;
//	for (int i = 0; i < imgsPath.size(); i++) //比较影像覆盖范围的大小
//	{
//		maxminXY img_XY;
//		vector<Point> image_poly;
//		img_XY = readImgXY(imgsPath[i]);
//		double Image_Xmin, Image_Xmax, Image_Ymin, Image_Ymax;
//
//		int area_count = 0;
//
//		Point point01, point02, point03, point04;
//		point01.x = img_XY.minX;
//		point01.y = img_XY.maxY;
//		image_poly.push_back(point01);
//
//		point02.x = img_XY.maxX;
//		point02.y = img_XY.maxY;
//		image_poly.push_back(point02);
//
//		point03.x = img_XY.maxX;
//		point03.y = img_XY.minY;
//		image_poly.push_back(point03);
//
//		point04.x = img_XY.minX;
//		point04.y = img_XY.minY;
//		image_poly.push_back(point04);
//
//		Image_Xmin = img_XY.minX;
//		Image_Xmax = img_XY.maxX;
//		Image_Ymin = img_XY.minY;
//		Image_Ymax = img_XY.maxY;
//
//		int ibegin = ((Image_Xmin - Xmin) / GridSpace_X) - 5;
//		int iend = ((Image_Xmax - Xmin) / GridSpace_X) + 5;
//		int jbegin = ((Image_Ymin - Ymin) / GridSpace_Y) - 5;
//		int jend = ((Image_Ymax - Ymin) / GridSpace_Y) + 5;
//		if (ibegin < 0)
//		{
//			ibegin = 0;
//		}
//		else if (ibegin > 500)
//		{
//			ibegin = 500;
//		}
//		if (iend < 0)
//		{
//			iend = 0;
//		}
//		else if (iend > 500)
//		{
//			iend = 500;
//		}
//		if (jbegin < 0)
//		{
//			jbegin = 0;
//		}
//		else if (jbegin > 500)
//		{
//			jbegin = 500;
//		}
//		if (jend < 0)
//		{
//			jend = 0;
//		}
//		else if (jend > 500)
//		{
//			jend = 500;
//		}
//		for (unsigned int i = ibegin; i != iend; i++)
//		{
//			for (unsigned int j = jbegin; j != jend; j++)
//			{
//				if ((PtInPolygon(pGrid[i][j].Left_Bottom_Point, image_poly, image_poly.size()) == true) &&
//					(PtInPolygon(pGrid[i][j].Left_Top_Point, image_poly, image_poly.size()) == true) &&
//					(PtInPolygon(pGrid[i][j].Right_Bottom_Point, image_poly, image_poly.size()) == true) &&
//					(PtInPolygon(pGrid[i][j].Right_Top_Point, image_poly, image_poly.size()) == true))
//				{
//					if (pGrid[i][j].Value == 1)
//					{
//						area_count++;
//					}
//				}
//			}
//		}
////		cout << area_count << endl;
//		count.push_back(area_count);
//	}
//
//	DestoryGrid(pGrid);
//
//	idx = sort_indexes(count); //对vector--count进行排序（从小到大），并返回索引值
//
//	for (int i = 0; i < imgsPath.size(); i++)//name=E587.9_N33.5_92552001
//	{
//		int end = imgsPath.size();
//		imgSortByArea.push_back(imgsPath[idx[end-i-1]]);//返回 从大到小的顺序
//	}
//
//	return imgSortByArea;
//}

vector<string> CImgSelectBFS::ImgSortByArea(vector<string>& imgsPath, string& shpfile)
{
	//获取区域
	vector<string> imgSortByArea;
	vector<Point> AreaPoint;
	vector<size_t> idx;
	vector<int> count;
	for (int i = 0; i < imgsPath.size(); i++) //比较影像覆盖范围的大小
	{
		GDALAllRegister();         //利用GDAL读取图片，先要进行注册
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径
		GDALDataset* m_pDataset = NULL;
		m_pDataset = (GDALDataset*)GDALOpen(imgsPath[i].c_str(), GA_ReadOnly);
		if (m_pDataset == NULL) {
			cout << "image open fail" << endl;
		}		
		int nNewWidth = m_pDataset->GetRasterXSize();
		int nNewHeight = m_pDataset->GetRasterYSize();

		int area_count = nNewWidth * nNewHeight;
		count.push_back(area_count);
	}

	idx = sort_indexes(count); //对vector--count进行排序（从小到大），并返回索引值
	for (int i = 0; i < imgsPath.size(); i++)//name=E587.9_N33.5_92552001
	{
		int end = imgsPath.size();
		imgSortByArea.push_back(imgsPath[idx[end - i - 1]]);//返回 从大到小的顺序
	}

	return imgSortByArea;
}


int CImgSelectBFS::ReadPolyFile(string &polyfile, vector<Point> &point)
{
	FILE *fp = NULL;
	fopen_s(&fp, polyfile.c_str(), "rb");
	if (fp == NULL)
	{
		return -1;
	}
	int num = 0;
	fread(&num, sizeof(num), 1, fp);
	for (int i = 0; i != num; i++)
	{
		Point tmp;
		fread(&tmp, sizeof(tmp), 1, fp);
		point.push_back(tmp);
	}
	fclose(fp);
	return 0;
}

int CImgSelectBFS::ResampleGDAL(string imgfile, string pszOutFile, float fResX, float fResY, GDALResampleAlg eResample)
{
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径
	GDALDataset *m_pDataset = NULL;
	m_pDataset = (GDALDataset *)GDALOpen(imgfile.c_str(), GA_ReadOnly);
	if (m_pDataset == NULL) {
		cout << "image open fail" << endl;
		return -1;
	}
	GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	if (pDriver == NULL)
	{
		GDALClose((GDALDatasetH)m_pDataset);
		return -2;
	}

	GDALDataType dataType = m_pDataset->GetRasterBand(1)->GetRasterDataType();

	char *pszSrcWKT = NULL;
	pszSrcWKT = const_cast<char *>(m_pDataset->GetProjectionRef());

	int nNewWidth = m_pDataset->GetRasterXSize();
	int nNewHeight = m_pDataset->GetRasterYSize();
	int m_bandNum = m_pDataset->GetRasterCount();
	double m_AdfGeoTransform[6];//投影转换参数
	m_pDataset->GetGeoTransform(m_AdfGeoTransform);

	bool bNoGeoRef = false;
	double dOldGeoTrans0 = m_AdfGeoTransform[0];
	//如果没有投影，人为设置一个    
	if (strlen(pszSrcWKT) <= 0)
	{
		//OGRSpatialReference oSRS;  
		//oSRS.SetUTM(50,true); //北半球  东经120度  
		//oSRS.SetWellKnownGeogCS("WGS587");  
		//oSRS.exportToWkt(&pszSrcWKT);  
		//pDSrc->SetProjection(pszSrcWKT);  
		//////////////////////////////////////////////////////////////////////////  
		m_AdfGeoTransform[0] = 1.0;
		m_pDataset->SetGeoTransform(m_AdfGeoTransform);
		//////////////////////////////////////////////////////////////////////////  
		bNoGeoRef = true;
	}
	//adfGeoTransform[0] /* top left x */  
	//adfGeoTransform[1] /* w-e pixel resolution */  
	//adfGeoTransform[2] /* rotation, 0 if image is "north up" */  
	//adfGeoTransform[3] /* top left y */  
	//adfGeoTransform[4] /* rotation, 0 if image is "north up" */  
	//adfGeoTransform[5] /* n-s pixel resolution */  

	m_AdfGeoTransform[1] = m_AdfGeoTransform[1] / fResX;
	m_AdfGeoTransform[5] = m_AdfGeoTransform[5] / fResY;
	nNewWidth = static_cast<int>(nNewWidth*fResX + 0.5);
	nNewHeight = static_cast<int>(nNewHeight*fResY + 0.5);


	//创建结果数据集  
	GDALDataset *pDDst = pDriver->Create(pszOutFile.c_str(), nNewWidth, nNewHeight, m_bandNum, dataType, NULL);
	if (pDDst == NULL)
	{
		GDALClose((GDALDatasetH)m_pDataset);
		return -2;
	}
	pDDst->SetProjection(pszSrcWKT);
	pDDst->SetGeoTransform(m_AdfGeoTransform);
	void *hTransformArg = NULL;
	hTransformArg = GDALCreateGenImgProjTransformer2((GDALDatasetH)m_pDataset, (GDALDatasetH)pDDst, NULL); //GDALCreateGenImgProjTransformer((GDALDatasetH) pDSrc,pszSrcWKT,(GDALDatasetH) pDDst,pszSrcWKT,FALSE,0.0,1);  
	if (hTransformArg == NULL)
	{
		GDALClose((GDALDatasetH)m_pDataset);
		GDALClose((GDALDatasetH)pDDst);
		return -3;
	}

	GDALWarpOptions *psWo = GDALCreateWarpOptions();
	psWo->papszWarpOptions = CSLDuplicate(NULL);
	psWo->eWorkingDataType = dataType;
	psWo->eResampleAlg = eResample;
	psWo->hSrcDS = (GDALDatasetH)m_pDataset;
	psWo->hDstDS = (GDALDatasetH)pDDst;
	psWo->pfnTransformer = GDALGenImgProjTransform;
	psWo->pTransformerArg = hTransformArg;
	psWo->nBandCount = m_bandNum;
	psWo->panSrcBands = (int *)CPLMalloc(m_bandNum * sizeof(int));
	psWo->panDstBands = (int *)CPLMalloc(m_bandNum * sizeof(int));
	for (int i = 0; i < m_bandNum; i++)
	{
		psWo->panSrcBands[i] = i + 1;
		psWo->panDstBands[i] = i + 1;
	}

	GDALWarpOperation oWo;
	if (oWo.Initialize(psWo) != CE_None)
	{
		GDALClose((GDALDatasetH)m_pDataset);
		GDALClose((GDALDatasetH)pDDst);
		return -3;
	}

	oWo.ChunkAndWarpImage(0, 0, nNewWidth, nNewHeight);

	GDALDestroyGenImgProjTransformer(hTransformArg);
	GDALDestroyWarpOptions(psWo);
	if (bNoGeoRef)
	{
		m_AdfGeoTransform[0] = dOldGeoTrans0;
		pDDst->SetGeoTransform(m_AdfGeoTransform);
		//pDDst->SetProjection("");  
	}
	GDALFlushCache(pDDst);
	GDALClose((GDALDatasetH)m_pDataset);
	GDALClose((GDALDatasetH)pDDst);
	return 0;
}

int CImgSelectBFS::img_dcloud(string imgfile, string pszOutFile)
{
	ResampleGDAL(imgfile, pszOutFile, 0.01, 0.01, GRA_Bilinear);
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径

	ThumbnailsDataSet = NULL;
	ThumbnailsDataSet = (GDALDataset*)GDALOpen(pszOutFile.c_str(), GA_ReadOnly);
	int m_imgWidth = ThumbnailsDataSet->GetRasterXSize();
	int m_imgHeight = ThumbnailsDataSet->GetRasterYSize();
	int m_imgBand = ThumbnailsDataSet->GetRasterCount();
	GDALRasterBand *poBand1;
	poBand1 = ThumbnailsDataSet->GetRasterBand(1);
	
	GUInt16 *pabyData = NULL;
	pabyData = new GUInt16[m_imgWidth*m_imgHeight];
	poBand1->RasterIO(GF_Read, 0, 0, m_imgWidth, m_imgHeight, pabyData, m_imgWidth, m_imgHeight, GDT_UInt16, 0, 0);
	//float value = iterate(pabyData, m_imgHeight, m_imgWidth);

	double mcloud = 0;
	int value = 150;
	//判断是否有云
	int black = 0;
	int cloud = 0;

	for (int i = 0; i != m_imgWidth; i++)
	{
		for (int j = 0; j != m_imgHeight; j++)
		{
			int dataValue = pabyData[j*m_imgWidth + i];
			
			if (dataValue == 0)
			{
				black++;
			}

			if (dataValue > value)
			{
				cloud++;;
			}			
		}
	}
	//计算云量
	mcloud = (cloud*1.0) / (m_imgWidth*m_imgHeight - black);

	if (pabyData) 
	{
		delete[] pabyData;
	}

	int cloudCount = mcloud * 100;
	return cloudCount;
}

float CImgSelectBFS::iterate(GUInt16 *image_in, int rows, int cols)
{
	GUInt16 *np;
	long hist[5874];
	int i, j, n;
	int ZMIN = 244, ZMAX = 0, TK = 0, TK1 = 5873;
	double ZOALL = 0, ZBALL = 0, ZBCONT = 0, ZOCONT = 0;
	// 对直方图置零...
	memset(hist, 0, sizeof(hist));
	// 生成直方图
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			np = &image_in[i*cols + j];
			hist[*np]++;
			if (*np > ZMAX) ZMAX = *np;
			if (*np < ZMIN) ZMIN = *np;

		}
	}
	hist[0] = 0;
	TK = (int)((ZMAX + ZMIN) / 2); //暂定分割阈值
	while (TK != TK1)
	{ //迭代运算直至最佳阈值
		TK1 = TK;
		for (n = 0; n < 5874; n++)
		{
			if (n <= TK)
			{
				ZBCONT += hist[n];
				ZBALL += n * hist[n];
			}
			else
			{
				ZOCONT += hist[n];
				ZOALL += n * hist[n];
			}
		}
		TK = ((ZBALL / ZBCONT) + (ZOALL / ZOCONT)) / 2;
	}
	return TK; //返回阈值
}

vector<string> CImgSelectBFS::interestingTimeSort(int targetTime, vector<string>& imgsPath, string & outimgPath)
{
	int timeDistance = 2000000;
	vector<int> timeDistance_box;
	vector<size_t> idx;
	vector<string> imgsByTimeSort;

	vector<string> imgsCloud5;
	vector<string> imgsCloud5_10;
	vector<string> imgsCloud10_20;
	vector<string> imgsCloud20;
	vector<int> timeDistance_boxCloud5;
	vector<int> timeDistance_boxCloud5_10;
	vector<int> timeDistance_boxCloud10_20;
	vector<int> timeDistance_boxCloud20;
	vector<size_t> idx_5;
	vector<size_t> idx_5_10;
	vector<size_t> idx_10_20;
	vector<size_t> idx_20;

	for (int i = 0; i < imgsPath.size(); i++)
	{
		int size = imageSize(imgsPath[i]);//输入影像的面积
		float size_T = float(size) / 200000000;
		int tmp_year = 0;
		int tmp_month = 0;
		int tmp_day = 0;
		int tmp_time = 0;

		//计算云量
		string imgName = imgsPath[i].substr(imgsPath[i].rfind("hubei"), imgsPath[i].find(".tif") - imgsPath[i].find("hubei"));
		/*string imgPath = imgPath + imgName + ".tif";*/
		string outImgPath = outimgPath +imgName+".tif" ;
		int cloud = img_dcloud(imgsPath[i], outImgPath);

		string time = imgsPath[i].substr(imgsPath[i].rfind("_201"), imgsPath[i].find("_L1") - imgsPath[i].find("_201"));
		string year = time.substr(1, 4);
		string month = time.substr(5, 2);
		string day = time.substr(7, 2);

		int year_int = atoi(year.c_str());

		tmp_year = (year_int - 2015) * 365;
		tmp_month = (atoi(month.c_str()) - 1) * 30;
		tmp_day = (atoi(day.c_str()));
		tmp_time = tmp_year + tmp_month + tmp_day;

		int tmp_timeDistance = abs(tmp_time - targetTime);
		int tmp_Distance = tmp_timeDistance / size_T;
		//timeDistance_box.push_back(tmp_Distance);
		cout << i << "--" << imgsPath.size();
		cout << "  size:" << size_T << "  timeD:" << tmp_timeDistance << "  tmp_Distance:" << tmp_Distance <<"  cloud:" << cloud << endl;

		if (cloud < 5)
		{
			imgsCloud5.push_back(imgsPath[i]);
			timeDistance_boxCloud5.push_back(tmp_Distance);
		}
		if ((cloud >= 5) && (cloud < 10))
		{
			imgsCloud5_10.push_back(imgsPath[i]);
			timeDistance_boxCloud5_10.push_back(tmp_Distance);
		}
		if ((cloud >= 10) && (cloud < 20))
		{
			imgsCloud10_20.push_back(imgsPath[i]);
			timeDistance_boxCloud10_20.push_back(tmp_Distance);
		}
		if ((cloud >= 20) )
		{
			imgsCloud20.push_back(imgsPath[i]);
			timeDistance_boxCloud20.push_back(tmp_Distance);
		}
		
	}

	idx_5= sort_indexes(timeDistance_boxCloud5);//对vector进行排序，并返回索引值
	for (int i = 0; i < timeDistance_boxCloud5.size(); i++)//name=E587.9_N33.5_92552001
	{
		string name = imgsCloud5[idx_5[i]].substr(imgsCloud5[idx_5[i]].rfind("E1"), imgsCloud5[idx_5[i]].find(".tif") - imgsCloud5[idx_5[i]].find("E1"));
		imgsByTimeSort.push_back(name);
	}

	idx_5_10 = sort_indexes(timeDistance_boxCloud5_10);
	for (int i = 0; i < timeDistance_boxCloud5_10.size(); i++)//name=E587.9_N33.5_92552001
	{
		string name = imgsCloud5_10[idx_5_10[i]].substr(imgsCloud5_10[idx_5_10[i]].rfind("E1"), imgsCloud5_10[idx_5_10[i]].find(".tif") - imgsCloud5_10[idx_5_10[i]].find("E1"));
		imgsByTimeSort.push_back(name);
	}

	idx_10_20 = sort_indexes(timeDistance_boxCloud10_20);
	for (int i = 0; i < timeDistance_boxCloud10_20.size(); i++)//name=E587.9_N33.5_92552001
	{
		string name = imgsCloud10_20[idx_10_20[i]].substr(imgsCloud10_20[idx_10_20[i]].rfind("E1"), imgsCloud10_20[idx_10_20[i]].find(".tif") - imgsCloud10_20[idx_10_20[i]].find("E1"));
		imgsByTimeSort.push_back(name);
	}
	
	idx_20 = sort_indexes(timeDistance_boxCloud20);
	for (int i = 0; i < timeDistance_boxCloud20.size(); i++)//name=E587.9_N33.5_92552001
	{
		string name = imgsCloud20[idx_20[i]].substr(imgsCloud20[idx_20[i]].rfind("E1"), imgsCloud20[idx_20[i]].find(".tif") - imgsCloud20[idx_20[i]].find("E1"));
		imgsByTimeSort.push_back(name);
	}

	return imgsByTimeSort;
}