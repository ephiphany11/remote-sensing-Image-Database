#include"CRectangleResizeImage.h"

CRectangleResizeImage::CRectangleResizeImage()
{
}
CRectangleResizeImage::~CRectangleResizeImage()
{
}

//1.统计影像中纵向连续值为0（1/2/3）的信息
vector<vector<position>> CRectangleResizeImage::staNumOfImgValue(string imgfile)
{
	vector<vector<position>> rectAngleArea;
	//存储薄云
	vector<position> rectAngleArea_minst_0;
	//存储优
	vector<position> rectAngleArea_good_1;
	//存储云
	vector<position> rectAngleArea_cloud_2;
	//存储背景
	vector<position> rectAngleArea_background_3;

	/*****************1. 读取影像,获取影像信息******************************/
	GDALAllRegister();         //利用GDAL读取图片，先要进行注册
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");   //设置支持中文路径														
	GDALDataset* poDataset1;//GDAL数据集							
	poDataset1 = (GDALDataset*)GDALOpen(imgfile.c_str(), GA_ReadOnly);

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
	const char* Pro = poDataset1->GetProjectionRef();

	unsigned char* pBuf = new unsigned char[m_imgWidth * m_imgHeight];
	memset(pBuf, 0, m_imgWidth * m_imgHeight * sizeof(unsigned char));
	poDataset1->RasterIO(GF_Read, 0, 0, m_imgWidth, m_imgHeight, pBuf, m_imgWidth, m_imgHeight, GDT_Byte, 1, 0, 0, 0, 0);

	/*****************2. 运用栈的特性，统计影像中连续值相同的信息******************************/
	//1.整景影像统计minst_0
	//int sumValue_minst0 = 1;
	//while (sumValue_minst0 != 0)
	//{
	//	sumValue_minst0 = 0;
	//	vector<position> rectAngleArea_minst0;
	//	for (int i = m_imgHeight - 1; i > -1; i--)
	//	{		
	//		int sumValue_row_minst0 = 1;
	//		vector<int> vec_minst0;
	//		for (int j = 0; j < m_imgWidth; j++)
	//		{
	//			//统计:从最后一行开始，每一列进行统计
	//			if (*(pBuf + i * m_imgWidth + j) == 0)
	//			{
	//				int num = 1;
	//				int tem = i;
	//				if ((tem - 1) > -1)
	//				{
	//					while (*(pBuf + (tem - 1) * m_imgWidth + j) == 0)
	//					{
	//						num++;
	//						tem--;
	//						if ((*(pBuf + (tem - 1) * m_imgWidth + j) != 0) || (tem < 1))
	//						{
	//							break;
	//						}
	//					}
	//				}					
	//				vec_minst0.push_back(num);
	//			}
	//			else {
	//				vec_minst0.push_back(0);
	//			}
	//		}
	//		sumValue_row_minst0 = accumulate(vec_minst0.begin(), vec_minst0.end(), 0);
	//		sumValue_minst0 += sumValue_row_minst0;
	//		if (sumValue_row_minst0 > 0)
	//		{
	//			position maxArea = MaxAreaImage(vec_minst0, i);
	//			int x_i = maxArea.center.x;
	//			int y_i = maxArea.center.y;
	//			int x_weight = maxArea.weight;
	//			int y_height = maxArea.height;
	//			rectAngleArea_minst0.push_back(maxArea);//求每一行的薄云最大面积
	//		}
	//		vec_minst0.clear();
	//	}
	//	if (sumValue_minst0 > 0)
	//	{
	//		position maxAreaofAllImage = MaxAreaofAllImage(rectAngleArea_minst0);
	//		int x_i = maxAreaofAllImage.center.x;
	//		int y_i = maxAreaofAllImage.center.y;
	//		int x_weight = maxAreaofAllImage.weight;
	//		int y_height = maxAreaofAllImage.height;
	//		rectAngleArea_minst_0.push_back(maxAreaofAllImage);//求整景影像中的薄云最大面积
	//		for (int j_x = x_i; j_x < x_i + x_weight; j_x++)
	//		{
	//			for (int j_y = 0; j_y < y_height; j_y++)
	//			{
	//				*(pBuf + (y_i - j_y) * m_imgWidth + j_x) = 10;
	//			}
	//		}
	//	}
	//	rectAngleArea_minst0.clear();
	//}
	//rectAngleArea.push_back(rectAngleArea_minst_0);

	//2.整景影像统计good_1
	int sumValue_good1 = 1;
	while (sumValue_good1 != 0)
	{
		sumValue_good1 = 0;
		vector<position> rectAngleArea_good1;
		for (int i = m_imgHeight - 1; i > -1; i--)
		{
			int sumValue_row_good1 = 1;
			vector<int> vec_good1;
			for (int j = 0; j < m_imgWidth; j++)
			{
				//统计:从最后一行开始，每一列进行统计
				if (*(pBuf + i * m_imgWidth + j) == 1)
				{
					int num = 1;
					int tem = i;
					if ((tem - 1) > -1)
					{
						while (*(pBuf + (tem - 1) * m_imgWidth + j) == 1)
						{
							num++;
							tem--;
							if ((*(pBuf + (tem - 1) * m_imgWidth + j) != 1) || (tem < 1))
							{
								break;
							}
						}
					}
					vec_good1.push_back(num);
				}
				else {
					vec_good1.push_back(0);
				}
			}
			sumValue_row_good1 = accumulate(vec_good1.begin(), vec_good1.end(), 0);
			sumValue_good1 += sumValue_row_good1;
			if (sumValue_row_good1 > 0)
			{
				position maxArea = MaxAreaImage(vec_good1, i);
				int x_i = maxArea.center.x;
				int y_i = maxArea.center.y;
				int x_weight = maxArea.weight;
				int y_height = maxArea.height;
				rectAngleArea_good1.push_back(maxArea);//求每一行的优最大面积
			}
			vec_good1.clear();
		}
		if (sumValue_good1 > 0)
		{
			position maxAreaofAllImage = MaxAreaofAllImage(rectAngleArea_good1);
			int x_i = maxAreaofAllImage.center.x;
			int y_i = maxAreaofAllImage.center.y;
			int x_weight = maxAreaofAllImage.weight;
			int y_height = maxAreaofAllImage.height;
			rectAngleArea_good_1.push_back(maxAreaofAllImage);//求整景影像中的优最大面积
			for (int j_x = x_i; j_x < x_i + x_weight; j_x++)
			{
				for (int j_y = 0; j_y < y_height; j_y++)
				{
					*(pBuf + (y_i - j_y) * m_imgWidth + j_x) = 11;
				}
			}
		}
		rectAngleArea_good1.clear();
	}
	rectAngleArea.push_back(rectAngleArea_good_1);

	//3.整景影像统计cloud2
	//int sumValue_cloud2 = 1;
	//while (sumValue_cloud2 != 0)
	//{
	//	sumValue_cloud2 = 0;
	//	vector<position> rectAngleArea_cloud2;
	//	for (int i = m_imgHeight - 1; i > -1; i--)
	//	{
	//		int sumValue_row_cloud2 = 1;
	//		vector<int> vec_cloud2;
	//		for (int j = 0; j < m_imgWidth; j++)
	//		{
	//			//统计:从最后一行开始，每一列进行统计
	//			if (*(pBuf + i * m_imgWidth + j) == 2)
	//			{
	//				int num = 1;
	//				int tem = i;
	//				if ((tem - 1) > -1)
	//				{
	//					while (*(pBuf + (tem - 1) * m_imgWidth + j) == 2)
	//					{
	//						num++;
	//						tem--;
	//						if ((*(pBuf + (tem - 1) * m_imgWidth + j) != 2) || (tem < 1))
	//						{
	//							break;
	//						}
	//					}
	//				}
	//				vec_cloud2.push_back(num);
	//			}
	//			else {
	//				vec_cloud2.push_back(0);
	//			}
	//		}
	//		sumValue_row_cloud2 = accumulate(vec_cloud2.begin(), vec_cloud2.end(), 0);
	//		sumValue_cloud2 += sumValue_row_cloud2;
	//		if (sumValue_row_cloud2 > 0)
	//		{
	//			position maxArea = MaxAreaImage(vec_cloud2, i);
	//			int x_i = maxArea.center.x;
	//			int y_i = maxArea.center.y;
	//			int x_weight = maxArea.weight;
	//			int y_height = maxArea.height;
	//			rectAngleArea_cloud2.push_back(maxArea);//求每一行的优最大面积
	//		}
	//		vec_cloud2.clear();
	//	}
	//	if (sumValue_cloud2 > 0)
	//	{
	//		position maxAreaofAllImage = MaxAreaofAllImage(rectAngleArea_cloud2);
	//		int x_i = maxAreaofAllImage.center.x;
	//		int y_i = maxAreaofAllImage.center.y;
	//		int x_weight = maxAreaofAllImage.weight;
	//		int y_height = maxAreaofAllImage.height;
	//		rectAngleArea_cloud_2.push_back(maxAreaofAllImage);//求整景影像中的优最大面积
	//		for (int j_x = x_i; j_x < x_i + x_weight; j_x++)
	//		{
	//			for (int j_y = 0; j_y < y_height; j_y++)
	//			{
	//				*(pBuf + (y_i - j_y) * m_imgWidth + j_x) = 12;
	//			}
	//		}
	//	}
	//	rectAngleArea_cloud2.clear();
	//}
	//rectAngleArea.push_back(rectAngleArea_cloud_2);

	//4.整景影像统计background3
	//int sumValue_background3 = 1;
	//while (sumValue_background3 != 0)
	//{
	//	sumValue_background3 = 0;
	//	vector<position> rectAngleArea_background3;
	//	for (int i = m_imgHeight - 1; i > -1; i--)
	//	{
	//		int sumValue_row_background3 = 1;
	//		vector<int> vec_background3;
	//		for (int j = 0; j < m_imgWidth; j++)
	//		{
	//			//统计:从最后一行开始，每一列进行统计
	//			if (*(pBuf + i * m_imgWidth + j) == 3)
	//			{
	//				int num = 1;
	//				int tem = i;
	//				if ((tem - 1) > -1)
	//				{
	//					while (*(pBuf + (tem - 1) * m_imgWidth + j) == 3)
	//					{
	//						num++;
	//						tem--;
	//						if ((*(pBuf + (tem - 1) * m_imgWidth + j) != 3) || (tem < 1))
	//						{
	//							break;
	//						}
	//					}
	//				}
	//				vec_background3.push_back(num);
	//			}
	//			else {
	//				vec_background3.push_back(0);
	//			}
	//		}
	//		sumValue_row_background3 = accumulate(vec_background3.begin(), vec_background3.end(), 0);
	//		sumValue_background3 += sumValue_row_background3;
	//		if (sumValue_row_background3 > 0)
	//		{
	//			position maxArea = MaxAreaImage(vec_background3, i);
	//			int x_i = maxArea.center.x;
	//			int y_i = maxArea.center.y;
	//			int x_weight = maxArea.weight;
	//			int y_height = maxArea.height;
	//			rectAngleArea_background3.push_back(maxArea);//求每一行的优最大面积
	//		}
	//		vec_background3.clear();
	//	}
	//	if (sumValue_background3 > 0)
	//	{
	//		position maxAreaofAllImage = MaxAreaofAllImage(rectAngleArea_background3);
	//		int x_i = maxAreaofAllImage.center.x;
	//		int y_i = maxAreaofAllImage.center.y;
	//		int x_weight = maxAreaofAllImage.weight;
	//		int y_height = maxAreaofAllImage.height;
	//		rectAngleArea_background_3.push_back(maxAreaofAllImage);//求整景影像中的优最大面积
	//		for (int j_x = x_i; j_x < x_i + x_weight; j_x++)
	//		{
	//			for (int j_y = 0; j_y < y_height; j_y++)
	//			{
	//				*(pBuf + (y_i - j_y) * m_imgWidth + j_x) = 13;
	//			}
	//		}
	//	}
	//	rectAngleArea_background3.clear();
	//}
	//rectAngleArea.push_back(rectAngleArea_background_3);
	return rectAngleArea;
}
//2.像元相邻值相同且连续进行合并
//vector<vector<position>> CRectangleResizeImage::MergeRectangleImage(vector<vector<vector<int>>>& vec)
//{
//	vector<vector<position>> rectAngleArea;
//	vector<position> rectAngleArea_mist_0;//输出符合条件的薄云区域
//	vector<position> rectAngleArea_good_1;//输出符合条件的优秀区域
//	vector<position> rectAngleArea_cloud_2;//输出符合条件的云区域
//	vector<position> rectAngleArea_background_3;//输出符合条件的背景区域
//	vector<vector<int>> mist_0 = vec[0];
//	vector<vector<int>> good_1 = vec[1];
//	vector<vector<int>> cloud_2 = vec[2];
//	vector<vector<int>> background_3 = vec[3];
//
//	//1.薄云区域统计
//	for (int j = 0; j < mist_0.size(); j++)
//	{
//		int n = mist_0[j].size();
//		vector<int> left(n), right(n);
//		stack<int> mono_stack;
//
//		for (int i = 0; i < n; ++i)
//		{
//			while (!mono_stack.empty() && mist_0[j][mono_stack.top()] >= mist_0[j][i])
//			{
//				right[mono_stack.top()] = i;
//				mono_stack.pop();
//			}
//			left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
//			mono_stack.push(i);
//		}
//
//		int area = 0;
//		int height = 0;
//		int weight = 0;
//		for (int i = 0; i < n; ++i)
//		{
//		    position centerPosition = { 0,
//										0,
//										0,
//										0,
//										0 };
//			height = mist_0[j][i];
//			weight = (right[i] - left[i] - 1);
//			area = weight * height;
//			if (area > 0)
//			{
//				centerPosition.center.x = i;
//				centerPosition.center.y = j;
//				centerPosition.left = left[i];
//				centerPosition.right = right[i];
//				centerPosition.high = mist_0[j][i];
//				rectAngleArea_mist_0.push_back(centerPosition);
//			}
//		}
//	}
//
//	//2.优区域统计
//	for (int j = 0; j < good_1.size(); j++)
//	{
//		int n = good_1[j].size();
//		vector<int> left(n), right(n);
//		stack<int> mono_stack;
//
//		for (int i = 0; i < n; ++i)
//		{
//			while (!mono_stack.empty() && good_1[j][mono_stack.top()] >= good_1[j][i])
//			{
//				right[mono_stack.top()] = i;
//				mono_stack.pop();
//			}
//			left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
//			mono_stack.push(i);
//		}
//
//		int area = 0;
//		int height = 0;
//		int weight = 0;
//		for (int i = 0; i < n; ++i)
//		{
//			position centerPosition = { 0,
//										0,
//										0,
//										0,
//										0 };
//			height = mist_0[j][i];
//			weight = (right[i] - left[i] - 1);
//			area = weight * height;
//			if (area > 0)
//			{
//				centerPosition.center.x = i;
//				centerPosition.center.y = j;
//				centerPosition.left = left[i];
//				centerPosition.right = right[i];
//				centerPosition.high = good_1[j][i];
//				rectAngleArea_good_1.push_back(centerPosition);
//			}
//		}
//	}
//	
//	//3.云区域统计
//	for (int j = 0; j < cloud_2.size(); j++)
//	{
//		int n = cloud_2[j].size();
//		vector<int> left(n), right(n);
//		stack<int> mono_stack;
//
//		for (int i = 0; i < n; ++i)
//		{
//			while (!mono_stack.empty() && cloud_2[j][mono_stack.top()] >= cloud_2[j][i])
//			{
//				right[mono_stack.top()] = i;
//				mono_stack.pop();
//			}
//			left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
//			mono_stack.push(i);
//		}
//
//		int area = 0;
//		int height = 0;
//		int weight = 0;
//		for (int i = 0; i < n; ++i)
//		{
//			position centerPosition = { 0,
//										0,
//										0,
//										0,
//										0 };
//			height = mist_0[j][i];
//			weight = (right[i] - left[i] - 1);
//			area = weight * height;
//			if (area > 0)
//			{
//				centerPosition.center.x = i;
//				centerPosition.center.y = j;
//				centerPosition.left = left[i];
//				centerPosition.right = right[i];
//				centerPosition.high = cloud_2[j][i];
//				rectAngleArea_cloud_2.push_back(centerPosition);
//			}
//		}
//	}
//
//	//4.背景区域统计
//	for (int j = 0; j < background_3.size(); j++)
//	{
//		int n = background_3[j].size();
//		vector<int> left(n), right(n);
//		stack<int> mono_stack;
//
//		for (int i = 0; i < n; ++i)
//		{
//			while (!mono_stack.empty() && cloud_2[j][mono_stack.top()] >= background_3[j][i])
//			{
//				right[mono_stack.top()] = i;
//				mono_stack.pop();
//			}
//			left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
//			mono_stack.push(i);
//		}
//
//		int area = 0;
//		int height = 0;
//		int weight = 0;
//		for (int i = 0; i < n; ++i)
//		{
//			position centerPosition = { 0,
//										0,
//										0,
//										0,
//										0 };
//			height = mist_0[j][i];
//			weight = (right[i] - left[i] - 1);
//			area = weight * height;
//			if (area > 0)
//			{
//				centerPosition.center.x = i;
//				centerPosition.center.y = j;
//				centerPosition.left = left[i];
//				centerPosition.right = right[i];
//				centerPosition.high = background_3[j][i];
//				rectAngleArea_background_3.push_back(centerPosition);
//			}
//		}
//	}
//
//	cout << rectAngleArea_mist_0.size()<<endl;
//	cout << rectAngleArea_good_1.size()<<endl;
//
//	rectAngleArea.push_back(rectAngleArea_mist_0);
//	rectAngleArea.push_back(rectAngleArea_good_1);
//	rectAngleArea.push_back(rectAngleArea_cloud_2);
//	rectAngleArea.push_back(rectAngleArea_background_3);
//
//	return rectAngleArea;
//}

//2.1连续且面积最大
position CRectangleResizeImage::MaxAreaImage(vector<int>& vec,int Y)
{
	stack<int> area_stack;
	vec.insert(vec.begin(), -1);//向数组两侧各插入一个比全局最小值还小的数字用以统一边界情况
	vec.push_back(-1);
	int len = vec.size();
	area_stack.push(0);
	int maxArea = 0;
	int height = 0;
	int weight = 0;
	int x_i = 0;

	for (int i = 1; i < len; i++)
	{
		while (vec[i] < vec[area_stack.top()])
		{
			int tmp = area_stack.top();
			area_stack.pop();
			int height_y = vec[tmp];
			int weight_x = (i - area_stack.top() - 1);
			int area = height_y * weight_x;

			if (area > maxArea)
			{
				maxArea = area;
				height = height_y;
				weight = weight_x;
				x_i = i- weight_x-1;
			}
		}
		area_stack.push(i);
	}
	//存储最大面积的位置信息
	position centerPosition;
	if (maxArea > 0)
	{
		centerPosition.center.x = x_i;
		centerPosition.center.y = Y;
		centerPosition.weight = weight;
		centerPosition.height = height;
	}

	return centerPosition;
}

//3.求整景影像最大面积
position CRectangleResizeImage::MaxAreaofAllImage(vector<position>& rowArea)
{
	int maxArea = 0;
	//存储最大面积的位置信息
	position centerPosition;
	for (int i=0; i<rowArea.size();i++)
	{
		int weight = rowArea[i].weight;
		int height = rowArea[i].height;
		int max_x = rowArea[i].center.x;
		int max_y = rowArea[i].center.y;
		int area = weight * height;

		if (area > maxArea)
		{
			maxArea = area;
			centerPosition.center.x = max_x;
			centerPosition.center.y = max_y;
			centerPosition.weight = weight;
			centerPosition.height = height;
		}
	}
	return centerPosition;
}

//4.将合并的区域裁剪
int CRectangleResizeImage::ResizeImage(string imgfile, string oriImgfile, string outpath)
{
	//2.统计矩形区域
	staRectangleArea = this->staNumOfImgValue(imgfile);
	vector<position> rectAngleArea_mist0;//输出符合条件的薄云区域
	vector<position> rectAngleArea_good1;//输出符合条件的优区域
	vector<position> rectAngleArea_cloud2;//输出符合条件的云区域
	vector<position> rectAngleArea_background3;//输出符合条件的背景区域
	/*rectAngleArea_mist0 = staRectangleArea[0];*/
	rectAngleArea_good1 = staRectangleArea[0];
	/*rectAngleArea_cloud2 = staRectangleArea[2];
	rectAngleArea_background3 = staRectangleArea[3];*/

	//裁剪后输出的路径
	vector<string>cutImgOutPath_mist_0;
	vector<string>cutImgOutPath_good_1;
	vector<string>cutImgOutPath_cloud_2;
	vector<string>cutImgOutPath_background_3;

	//int num_mist_0 = rectAngleArea_mist0.size();
	//for (int i = 0; i < num_mist_0; i++)
	//{
	//	string tmp = imgfile.substr(imgfile.rfind("\\"), imgfile.rfind(".") - imgfile.rfind("\\"));//读文件名称
	//	string path = outpath + tmp + "_minst4_" + to_string(i) + ".tiff";
	//	cutImgOutPath_mist_0.push_back(path);
	//}

	int num_good_1 = rectAngleArea_good1.size();
	for (int i = 0; i < num_good_1; i++)
	{
		string tmp = imgfile.substr(imgfile.rfind("\\"), imgfile.rfind(".") - imgfile.rfind("\\"));//读文件名称
		string path = outpath + tmp + "_good1_" + to_string(i) + ".tiff";
		cutImgOutPath_good_1.push_back(path);
	}

	//int num_cloud_2 = rectAngleArea_cloud2.size();
	//for (int i = 0; i < num_cloud_2; i++)
	//{
	//	string tmp = imgfile.substr(imgfile.rfind("\\"), imgfile.rfind(".") - imgfile.rfind("\\"));//读文件名称
	//	string path = outpath + tmp + "_cloud2_" + to_string(i) + ".tiff";
	//	cutImgOutPath_cloud_2.push_back(path);
	//}

	//int num_background_3 = rectAngleArea_background3.size();
	//for (int i = 0; i < num_background_3; i++)
	//{
	//	string tmp = imgfile.substr(imgfile.rfind("\\"), imgfile.rfind(".") - imgfile.rfind("\\"));//读文件名称
	//	string path = outpath + tmp + "_background3_" + to_string(i) + ".tiff";
	//	cutImgOutPath_background_3.push_back(path);
	//}

	//3.裁剪符合要求的矩形区域，并输出
	/*for (int i = 0; i < num_mist_0; i++)
	{
		int startX = rectAngleArea_mist0[i].center.x;
		int startY = rectAngleArea_mist0[i].center.y - rectAngleArea_mist0[i].height+1;
		int sizeX = rectAngleArea_mist0[i].weight;
		int sizeY = rectAngleArea_mist0[i].height;

		this->ImageCut(oriImgfile.c_str(), cutImgOutPath_mist_0[i].c_str(), startX, startY, sizeX, sizeY, "Gtiff");
	}*/

	for (int i = 0; i < num_good_1; i++)
	{
		int startX = rectAngleArea_good1[i].center.x;
		int startY = rectAngleArea_good1[i].center.y - rectAngleArea_good1[i].height+1;
		int sizeX = rectAngleArea_good1[i].weight;
		int sizeY = rectAngleArea_good1[i].height;

		this->ImageCut(oriImgfile.c_str(), cutImgOutPath_good_1[i].c_str(), startX, startY, sizeX, sizeY, "Gtiff");
	}

	/*for (int i = 0; i < num_cloud_2; i++)
	{
		int startX = rectAngleArea_cloud2[i].center.x;
		int startY = rectAngleArea_cloud2[i].center.y - rectAngleArea_cloud2[i].height+1;
		int sizeX = rectAngleArea_cloud2[i].weight;
		int sizeY = rectAngleArea_cloud2[i].height;

		this->ImageCut(oriImgfile.c_str(), cutImgOutPath_cloud_2[i].c_str(), startX, startY, sizeX, sizeY, "Gtiff");
	}*/

	/*for (int i = 0; i < num_background_3; i++)
	{
		int startX = rectAngleArea_background3[i].center.x;
		int startY = rectAngleArea_background3[i].center.y - rectAngleArea_background3[i].height+1;
		int sizeX = rectAngleArea_background3[i].weight;
		int sizeY = rectAngleArea_background3[i].height;

		this->ImageCut(oriImgfile.c_str(), cutImgOutPath_background_3[i].c_str(), startX, startY, sizeX, sizeY, "Gtiff");
	}*/
	
	return 0;
}

void CRectangleResizeImage::ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY, const char* pszFormat)
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
	adfGeoTransform[0] = adfGeoTransform[0] + iStartX * adfGeoTransform[1] + iStartY * adfGeoTransform[2];
	adfGeoTransform[3] = adfGeoTransform[3] + iStartX * adfGeoTransform[4] + iStartY * adfGeoTransform[5];

	// 创建输出文件并设置空间参考和坐标信息
	GDALDriver* poDriver = (GDALDriver*)GDALGetDriverByName(pszFormat);
	GDALDataset* pDstDS = poDriver->Create(pszDstFile, iDstWidth, iDstHeight, iBandCount, eDT, NULL);
	pDstDS->SetGeoTransform(adfGeoTransform);
	pDstDS->SetProjection(pSrcDS->GetProjectionRef());

	int* pBandMap = new int[iBandCount];
	for (int i = 0; i < iBandCount; i++)
	{
		pBandMap[i] = i + 1;
	}

	// 下面这里需要根据数据类型来判断，申请不同类型的缓存来进行处理，这里仅仅以8bit图像进行演示
	if (eDT == GDT_Byte) // 如果是8bit图像
	{
		// 申请所有数据所需要的缓存，如果图像太大应该用分块处理
		unsigned char* pDataBuff = new unsigned char[iDstWidth * iDstHeight * iBandCount];
		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		//避免薄雾-0，输出的值全为0
		for (int i = 0; i < iDstHeight; i++)
		{
			for (int j = 0; j < iDstWidth; j++)
			{
				if (*(pDataBuff + i * iDstWidth + j) == 0)
				{
					*(pDataBuff+ i * iDstWidth + j) = 4;
				}
			}
		}
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		delete(pDataBuff);
	}
	else// 如果是16bit图像
	{
		// 申请所有数据所需要的缓存，如果图像太大应该用分块处理
		unsigned short* pDataBuff = new unsigned short[iDstWidth * iDstHeight * iBandCount];
		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		//避免薄雾-0，输出的值全为0
		for (int i = 0; i < iDstHeight; i++)
		{
			for (int j = 0; j < iDstWidth; j++)
			{
				if (*(pDataBuff + i * iDstWidth + j) == 0)
				{
					*(pDataBuff + i * iDstWidth + j) = 4;
				}
			}
		}
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		delete(pDataBuff);
	}

	delete(pBandMap);
	GDALClose((GDALDatasetH)pSrcDS);
	GDALClose((GDALDatasetH)pDstDS);

}
