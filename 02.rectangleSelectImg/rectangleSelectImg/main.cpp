#include<stdio.h>
#include<stack>
#include<vector>
#include<iostream>
#include<algorithm>
//#include "CRectangleArea.h"
#include"CRectangleResizeImage.h"
#include "CImgScreen.h"
#include "CfileSelect.h"
#include "ogrsf_frmts.h"
#include "cpl_string.h" 
using namespace std;

int main()
{
	
/*************02裁剪输出符合要求的区域*************************/

	//选择特定文件
	string oriImgPath = "H:\\Data.20230911\\02_CB04.10m.Hubei\\05.DL.result";
	string inPutImgPath = "H:\\Data.20230911\\02_CB04.10m.Hubei\\05.DL.result";
	string outPutImgPath = "H:\\Data.20230911\\02_CB04.10m.Hubei\\06.rectangleImg_Good";
	/*string oriImgPath = "F:\\GF-6Data\\GF-6\\04\\1.gf-6.Data.05-01\\";
	string inPutImgPath = "F:\\GF-6Data\\GF-6\\04\\2.gf-6.regionalQualityGrading05-01\\";
	string outPutImgPath = "F:\\GF-6Data\\GF-6\\04\\3.gf-6.DL.ImgSelect.Hubei05\\";*/
	vector<string> oriImg_list;//存放原始影像的路径
	vector<string> inPutImg_list;//存放输入影像的路径

	CfileSelect fileSelect;
	fileSelect.get_filelist_format(oriImgPath, inPutImgPath, outPutImgPath, oriImg_list, inPutImg_list, "*.tif");

	/*int count = 0;
	for (int i = 0; i < oriImg_list.size(); i++)
	{		
	
		CRectangleArea RectangleArea(inPutImg_list[i].c_str(), oriImg_list[i].c_str(), outPutImgPath.c_str());
		count++;
		cout << "  第" << count << "景处理完成GoodJop!!" << endl;
	}*/
	int count = 0;
	for (int i = 0; i < oriImg_list.size(); i++)
	{
		CRectangleResizeImage RectangleResizeImage;
		RectangleResizeImage.ResizeImage(inPutImg_list[i].c_str(), oriImg_list[i].c_str(), outPutImgPath.c_str());
		count++;
		cout << "  第" << count << "景处理完成GoodJop!!" << endl;
	}

/*************01将覆盖武汉市区域的影像挑选出来*************************/
	//int num = 0;
	////选择特定文件
	//string dstFolder = "G:\\cly-GF6-hubei-20230316";
	//string path = "H:\\GF6-WFV\\全国一月份GF6-WFV-三波段-校正后-16bit\\第3批-16\\afterCorrection";
	//vector<string> file_list;//存放子文件夹中的指定文件

	//CfileSelect fileSelect1;
	//fileSelect1.fileSearch(path, file_list);

	///*cout << file_list.size() << endl;
	//for (int i = 0; i < file_list.size(); i++)
	//{
	//	cout << file_list[i] << endl;
	//}*/

	////筛选覆盖武汉市区域的GF-1影像
	//CImgScreen ImgScreen;
	//for (int i = 0; i < file_list.size(); i++)
	//{
	//	if (ImgScreen.ImgScreen(file_list[i]))
	//	{
	//		//拷贝文件夹
	//		fileSelect1.copyFolder(file_list[i], dstFolder);
	//		cout << "当前第  " << i << "景,  " << "拷贝第  " << num << "  景覆盖湖北的影像" << endl;
	//		num++;
	//	}
	//	else {
	//		continue;
	//	}
	//}

/*************03写shp文件*************************/
	//using namespace std;
	//void convertToShp(double longitude, double latitude, char *outshp)
	//{
	//	//使属性表字段支持中文
	//	CPLSetConfigOption("SHAPE_ENCODING", "");
	//	OGRRegisterAll();//注册所有的驱动
	//					 //创建ESRI shp文件
	//	char *pszDriverName = "ESRI Shapefile";
	//	//调用对Shape文件读写的Driver
	//	OGRSFDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
	//	if (poDriver == NULL)
	//	{
	//		cout << pszDriverName << "驱动不可用！" << endl;
	//		return;
	//	}
	//	//创建数据源
	//	OGRDataSource *poDs = poDriver->CreateDataSource(outshp, NULL);
	//	if (poDs == NULL)
	//	{
	//		cout << "DataSource Creation Error" << endl;
	//		return;
	//	}
	//	//创建图层Layer
	//	string outShapName = outshp;
	//	string layerName = outShapName.substr(0, outShapName.length() - 4);
	//	//layerName.c_str()表示将string转为char *类型
	//	//参数说明：新图层名称，坐标系，图层的几何类型，创建选项，与驱动有关
	//	OGRLayer *poLayer = poDs->CreateLayer(layerName.c_str(), NULL, wkbPoint, NULL);
	//	if (poLayer == NULL)
	//	{
	//		cout << "Layer Creation Failed" << endl;
	//		OGRDataSource::DestroyDataSource(poDs);
	//		return;
	//	}
	//	//下面创建属性表，我们在属性表中创建两列数据即可
	//	//先创建一个“ID”整型属性
	//	OGRFieldDefn oFieldId("ID", OFTInteger);
	//	oFieldId.SetWidth(5);
	//	poLayer->CreateField(&oFieldId);
	//	//再创建一个"X"double属性
	//	OGRFieldDefn oFieldX("X", OFTString);
	//	oFieldX.SetWidth(30);
	//	poLayer->CreateField(&oFieldX);
	//	//再创建一个"Y"double属性
	//	OGRFieldDefn oFieldY("Y", OFTString);
	//	oFieldY.SetWidth(30);
	//	poLayer->CreateField(&oFieldY);
	//	//创建一个feature
	//	OGRFeature *poFeature;
	//	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());//GetLayerDefn()获取当前图层的属性表结构
	//																   //给属性表中我们刚创建的列赋值
	//	int i = 0;
	//	poFeature->SetField("ID", i);
	//	poFeature->SetField("X", longitude);
	//	poFeature->SetField("Y", latitude);
	//	i++;
	//	//创建一个OGRPoint对象
	//	OGRPoint point;
	//	point.setX(longitude);
	//	point.setY(latitude);
	//	//point.setZ(0);

	//	poFeature->SetGeometry(&point);

	//	if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
	//	{
	//		printf("Failed to create feature in shapefile.\n");
	//		exit(1);
	//	}
	//	OGRFeature::DestroyFeature(poFeature);
	//	OGRDataSource::DestroyDataSource(poDs);
	//}
	////======= 经纬度转化为投影坐标=============
	//double* transfer(double longitude, double latitude)
	//{
	//	OGRSpatialReference oSourceSRS;
	//	//EPSG code 代表特定的椭球体、单位、地理坐标系或投影坐标系等信息
	//	//This method will initialize the spatial reference based on the passed in EPSG GCS or PCS code
	//	oSourceSRS.importFromEPSG(4326);//EPSG:4326代表地理坐标系WGS1984
	//	OGRSpatialReference oTargetSRS;
	//	oTargetSRS.importFromEPSG(2029);// utm zone 17N
	//	OGRCoordinateTransformation *poTransform;
	//	poTransform = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
	//	if (poTransform == NULL)
	//	{
	//		cout << "poTransform is null" << endl;
	//		exit(1);
	//	}
	//	if (!poTransform->Transform(1, &longitude, &latitude, NULL))
	//	{
	//		cout << "transform failed" << endl;
	//		exit(1);
	//	}
	//	//poTransform->Transform(1, &longitude, &latitude, NULL);
	//	double *inout = new double[2];
	//	inout[0] = longitude;
	//	inout[1] = latitude;
	//	return inout;
	//}
	//int main(int argc, char *argv[])
	//{
	//	char *xCoordinate = argv[1];
	//	char *yCoordinate = argv[2];
	//	char *outShp = argv[3];
	//	double x = atof(xCoordinate);
	//	double y = atof(yCoordinate);
	//	convertToShp(x, y, outShp);
	//	cout << "success! file is saved to " << outShp << endl;
	//	double *transferedLongLat = transfer(116.246742, 40.022211);
	//	cout << "转换后的投影坐标为：" << transferedLongLat[0] << "," << transferedLongLat[1] << endl;

	//	getchar();
	//}

	return 0;
}


