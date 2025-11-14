#include "CRectangleArea.h"



CRectangleArea::CRectangleArea()
{
}

CRectangleArea::CRectangleArea(string imgfile, string oriImgfile, string outpath)
{
	//1.统计为某一类的个数
	statistics = this->statisticsNumOfImg(imgfile);
	//2.统计矩形区域
	staRectangleArea = this->RectangleArea(statistics);
	//裁剪后输出的路径
	vector<string>cutImgOutPath;
	int num = staRectangleArea.size();
	for (int i = 0; i < num; i++)
	{
		string tmp = imgfile.substr(imgfile.rfind("\\"), imgfile.rfind(".") - imgfile.rfind("\\"));//读文件名称
		string path = outpath +tmp +"_"+to_string(i) + ".tiff";
		cutImgOutPath.push_back(path);
	}
	//3.裁剪符合要求的矩形区域，并输出
	
	for (int i = 0; i < num; i++)
	{
		int startX = (staRectangleArea[i].left+1)*224;
		int startY = (staRectangleArea[i].center.y- staRectangleArea[i].high+1)*224;
		int sizeX = (staRectangleArea[i].right - staRectangleArea[i].left - 1)*224;
		int sizeY= (staRectangleArea[i].high)*224;

		this->ImageCut(oriImgfile.c_str(), cutImgOutPath[i].c_str(), startX, startY, sizeX, sizeY,"Gtiff");
	}
}


CRectangleArea::~CRectangleArea()
{
}

vector<position> CRectangleArea::RectangleArea(vector<vector<int>>& heights)
{
	vector<position> rectangleArea;
	vector<position> rectAngleArea;//输出符合条件的区域
	for (int j = 0; j < heights.size(); j++)
	{
		int n = heights[j].size();
		vector<int> left(n), right(n);
		stack<int> mono_stack;

		for (int i = 0; i < n; ++i)
		{
			while (!mono_stack.empty() && heights[j][mono_stack.top()] >= heights[j][i])
			{
				right[mono_stack.top()] = i;
				mono_stack.pop();
			}
			left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
			mono_stack.push(i);
		}

		int area = 0;
		int height = 0;
		int weight = 0;
		for (int i = 0; i < n; ++i)
		{
			centerPosition = {  0,
				                0,
				                0,
								0,
								0  };
			height = heights[j][i];
			weight = (right[i] - left[i] - 1);
			area = weight * height;
			if (area > 0 /*&& height >1 && weight>1*/)
			{
				centerPosition.center.x = i;
				centerPosition.center.y = j;
				centerPosition.left = left[i];
				centerPosition.right = right[i];
				centerPosition.high = heights[j][i];
				rectangleArea.push_back(centerPosition);
			}
		}
	}

	//删除重复的区域	
	int count= rectangleArea.size();
	
	for (int j = 0; j < count; j++)
	{
		bool tmp = true;
		for (int i = j+1; i < count; i++)
		{
			int startX01 = rectangleArea[j].left + 1;
			int startY01 = rectangleArea[j].center.y - rectangleArea[j].high + 1;
			int startX02 = rectangleArea[i].left + 1;
			int startY02 = rectangleArea[i].center.y - rectangleArea[i].high + 1;

			int sizeX01 = rectangleArea[j].right - rectangleArea[j].left - 1;
			int sizeY01 = rectangleArea[j].high;
			int sizeX02 = rectangleArea[i].right - rectangleArea[i].left - 1;
			int sizeY02 = rectangleArea[i].high;

			int endX01 = startX01 + sizeX01;
			int endY01 = startY01 + sizeY01;
			int endX02 = startX02 + sizeX02;
			int endY02 = startY02 + sizeY02;

			if (startX01 >= startX02 && startY01 >= startY02 && endX01 <= endX02 && endY01 <= endY02)
			{			
				tmp = false;
				break;
			}
	    }

		if (tmp)
		{
			rectAngleArea.push_back(rectangleArea[j]);
		}

      }
	return rectAngleArea;
}

vector<vector<int>> CRectangleArea::statisticsNumOfImg(string inPutImgPath)
{
	vector<int> vec;
	vector<vector<int>> vecc01;
	vector<vector<int>> vecc02;

	/*****************1. 读取影像,获取影像信息******************************/
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径														
	GDALDataset *poDataset1;//GDAL数据集							
	poDataset1 = (GDALDataset*)GDALOpen(inPutImgPath.c_str(), GA_ReadOnly);

	//确认影像是否读入	
	if (poDataset1 == NULL)
	{
		cout << "影像打开失败!" << endl;
	}
	else {
		cout << "影像打开成功!" << endl;
	}

	//获取影像的长、宽、波段数及投影信息
	m_imgBand = poDataset1->GetRasterCount();//这里为1
	m_imgWidth = poDataset1->GetRasterXSize();
	m_imgHeight = poDataset1->GetRasterYSize();

	//图像坐标信息	
	poDataset1->GetGeoTransform(m_GeoTransform);
	const char *Pro = poDataset1->GetProjectionRef();

	unsigned char* pBuf = new unsigned char[m_imgWidth * m_imgHeight];
	memset(pBuf, 0, m_imgWidth * m_imgHeight * sizeof(unsigned char));
	poDataset1->RasterIO(GF_Read, 0, 0, m_imgWidth, m_imgHeight, pBuf, m_imgWidth, m_imgHeight, GDT_Byte, 1, 0, 0, 0, 0);

	/*for (int i = 0; i < m_imgHeight; i++)
	{
		for (int j = 0; j < m_imgWidth; j++)
		{
			unsigned char* value2 = pBuf + i*m_imgWidth + j;
			unsigned char tep =*value2 ;
			cout << tep << " ";
		}
		cout << endl;
	}*/

	/*****************2. 运用栈的特性，统计影像中纵向连续值为1的信息******************************/
	for (int i = m_imgHeight -1; i >-1 ; i--)
	{
		for (int j = 0; j < m_imgWidth; j++)
		{
			/*if ((*(pBuf + i*m_imgWidth + j) == 1) || (*(pBuf + i*m_imgWidth + j) == 0))*/
			if (*(pBuf + i * m_imgWidth + j) == 1)
			{
				int num = 1;
				int tem = i;
				/*while ((*(pBuf + (tem-1)*m_imgWidth + j) == 1) || (*(pBuf + (tem - 1)*m_imgWidth + j) == 0))
				{									
					num++;				
					tem--;
					if (((*(pBuf + (tem - 1)*m_imgWidth + j) != 1) && (*(pBuf + (tem - 1)*m_imgWidth + j) != 0))||(tem < 0))
					{
						break;
					}
				}*/
				while (*(pBuf + (tem - 1) * m_imgWidth + j) == 1)
				{
					num++;
					tem--;
					if ((*(pBuf + (tem - 1) * m_imgWidth + j) != 1) || (tem < 0))
					{
						break;
					}
				}
				vec.push_back(num);
			}
			else {
				vec.push_back(0);
			}
		}
		vecc01.push_back(vec);
		vec.clear();
	}


	for (int i = 0; i < vecc01.size(); i++)
	{		
	 vecc02.push_back( vecc01[vecc01.size()-i-1]);				
	}
	
	return vecc02;
}

void CRectangleArea::ImageCut(const char * pszSrcFile, const char * pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY, const char * pszFormat)
{
	GDALAllRegister();
	GDALDataset* pSrcDS = (GDALDataset*)GDALOpen(pszSrcFile, GA_ReadOnly);
	GDALDataType eDT = pSrcDS->GetRasterBand(1)->GetRasterDataType();

	int iBandCount = pSrcDS->GetRasterCount();

	// 根据裁切范围确定裁切后的图像宽高
	int iDstWidth = iSizeX;
	int iDstHeight = iSizeY;
	double adfGeoTransform[6] = { 0 };
	pSrcDS->GetGeoTransform(adfGeoTransform);

	// 计算裁切后的图像的左上角坐标
	adfGeoTransform[0] = adfGeoTransform[0] + iStartX*adfGeoTransform[1] + iStartY*adfGeoTransform[2];
	adfGeoTransform[3] = adfGeoTransform[3] + iStartX*adfGeoTransform[4] + iStartY*adfGeoTransform[5];

	// 创建输出文件并设置空间参考和坐标信息
	GDALDriver* poDriver = (GDALDriver *)GDALGetDriverByName(pszFormat);
	GDALDataset *pDstDS = poDriver->Create(pszDstFile, iDstWidth, iDstHeight, iBandCount, eDT, NULL);
	pDstDS->SetGeoTransform(adfGeoTransform);
	pDstDS->SetProjection(pSrcDS->GetProjectionRef());

	int *pBandMap = new int[iBandCount];
	for (int i = 0; i < iBandCount; i++)
	{
			pBandMap[i] = i + 1;
	}

	// 下面这里需要根据数据类型来判断，申请不同类型的缓存来进行处理，这里仅仅以8bit图像进行演示
	if (eDT == GDT_Byte) // 如果是8bit图像
	{
		// 申请所有数据所需要的缓存，如果图像太大应该用分块处理
		unsigned char *pDataBuff = new unsigned char[iDstWidth*iDstHeight*iBandCount];
		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		delete(pDataBuff);
	}
	else// 如果是16bit图像
	{
		// 申请所有数据所需要的缓存，如果图像太大应该用分块处理
		unsigned short *pDataBuff = new unsigned short[iDstWidth*iDstHeight*iBandCount];
		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		delete(pDataBuff);
	}

	delete(pBandMap);
	GDALClose((GDALDatasetH)pSrcDS);
	GDALClose((GDALDatasetH)pDstDS);

}
