#include "CfileSelect.h"


CfileSelect::CfileSelect()
{
}

CfileSelect::~CfileSelect()
{
}


//转换 字符串类型==》TCAHR类型
TCHAR* CfileSelect:: char2TCAHR(const char* str)
{
	int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	TCHAR* retStr = new TCHAR[size + 1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, retStr, size);
	retStr[size] = '\0';
	return retStr;
}

//复制文件夹到指定的路径
void CfileSelect::copyFolder(string srcFolder, string dstFolder)
{

	//string srcFolder = "D:\\7Zip\\7-Zip\\Lang";
	////char srcFolder[100] = "D:\\7Zip\\7-Zip\\Lang";
	//char dstFolder[100] = "D:\\BaiduNetdiskDownload";
	SHFILEOPSTRUCT fop = { 0 };
	fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	fop.wFunc = FO_COPY;//选择执行类型，FO_COPY,FO_DELETE,FO_RENAME,FO_MOVE四种

	fop.pFrom = char2TCAHR(srcFolder.c_str());//如：D:\\*.txt
	fop.pTo = char2TCAHR(dstFolder.c_str());//D:\\test

	SHFileOperation(&fop);
}

void CfileSelect::fileSearch(string path, vector<string> &imgs)
{
	long long hFile = 0;
	/*
	_finddata_t  存储文件各种信息的结构体，<io.h>;
	*/
	struct _finddata_t fileInfo;
	string pathName;
	/*
	\\* 表示符合的所有文件;
	没有找到即文件夹为空，退出;
	assign 表示把 pathName清空并置为path;
	append 表示在末尾加上字符串;
	c_str 返回一个const char* 的临时指针;
	_findfirst
	搜索与指定的文件名称匹配的第一个实例，若成功则返回第一个实例的句柄，否则返回-1L;
	函数原型：long _findfirst( char *filespec, struct _finddata_t *fileinfo );
	*/
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1)
		return;

	do {
		string tmp = path + "\\" + fileInfo.name;
		string tmp2 = tmp.substr(tmp.rfind("."), tmp.length());
		if (tmp.substr(tmp.rfind(".") + 1, tmp.length()) == "TIF" || tmp.substr(tmp.rfind(".") + 1, tmp.length()) == "TIF")
		{
			imgs.push_back(tmp);
		}
		if (tmp.substr(tmp.rfind(".") + 1, tmp.length()) == "tiff" || tmp.substr(tmp.rfind(".") + 1, tmp.length()) == "tiff")
		{
			imgs.push_back(tmp);
		}
		//	cout << path + "\\" + fileInfo.name << endl;
		/*
		文件夹下有 . 和 .. 目录，不能进入搜索;
		_A_SUBDIR 表示文件夹属性;
		*/
		/*if (strcmp(fileInfo.name, "..") && strcmp(fileInfo.name, ".") && fileInfo.attrib == _A_SUBDIR)
		fileSearch(path + "\\" + fileInfo.name);*/
	} while (_findnext(hFile, &fileInfo) == 0);
	/*
	_findnext 搜索与_findfirst函数提供的文件名称匹配的下一个实例，若成功则返回0，否则返回－1 ;
	_findclose 结束查找;
	*/
	_findclose(hFile);
	return;
}


void CfileSelect::get_filelist_format(string oriImgPath, string inPutImgPath, string outPutImgPath, vector<string>& oriImg_list, vector<string>& inPutImg_list, string format)
{
	long long hFile = 0;
	/*
	_finddata_t  存储文件各种信息的结构体，<io.h>;
	*/
	struct _finddata_t fileInfo;
	string pathName;
	/*
	\\* 表示符合的所有文件;
	没有找到即文件夹为空，退出;
	assign 表示把 pathName清空并置为path;
	append 表示在末尾加上字符串;
	c_str 返回一个const char* 的临时指针;
	_findfirst搜索与指定的文件名称匹配的第一个实例，若成功则返回第一个实例的句柄，否则返回-1L;
	函数原型：long _findfirst( char *filespec, struct _finddata_t *fileinfo );
	*/
	if ((hFile = _findfirst(pathName.assign(inPutImgPath).append("\\*").c_str(), &fileInfo)) == -1)
		return;

	do {
		string tmp = inPutImgPath + "\\" + fileInfo.name;
		string name = tmp.substr(tmp.rfind("\\"), tmp.rfind(".") - tmp.rfind("\\"));
		string tmp2 = oriImgPath + "\\" + name + ".tif";

		if (tmp.substr(tmp.rfind(".") + 1, tmp.length()) == "tif")
		{
			inPutImg_list.push_back(tmp);
			oriImg_list.push_back(tmp2);
		}
	} while (_findnext(hFile, &fileInfo) == 0);

	_findclose(hFile);
	return;
}