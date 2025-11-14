#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
#include <windows.h>
#include <shellapi.h>

using namespace std;

class CfileSelect
{
public:
	CfileSelect();
	~CfileSelect();

	//获取子文件夹，获取某一特定后缀的所有文件
	void fileSearch(string path, vector<string> &imgs);
	
	//复制文件夹
	void copyFolder(string srcFolder, string dstFolder);
	
	//转换 字符串类型==》TCAHR类型
	TCHAR* char2TCAHR(const char* str);
	
	void CfileSelect::get_filelist_format(string oriImgPath, string inPutImgPath, string outPutImgPath, vector<string>& oriImg_list, vector<string>& inPutImg_list, string format);
};

